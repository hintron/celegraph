---
id: server_theory
title: How Celegraph works - high level
---

Currently, there is a single central server that manages everything. All messages between clients are routed through this server.

The server creates a pool of worker threads that each can process a single packet at a time. The main thread listens for packets and forwards those packets to the worker threads via a single packet queue.


How Celegraph works - more details:
------------
When the server first starts (see `Server::Run()` in Server.cpp), the main thread spawns an admin shell thread:
```
main
|       admin shell
|-------^
|       |
```
This shell provides administrative CLI access to the server itself during operation (see AdminShell.cpp). It just loops, waiting for input from the command line.

Once the admin shell thread has been created, the main thread (i.e. `Server::Run()`) will then create worker threads (see `Server::WorkerThread()` in Server.cpp):
```
main
|       admin shell
|-------^
|       |
|       |     wrk1    wrk2    wrk3    wrk4
|---------------^-------^-------^-------^
|       |       |       |       |       |
|       |       |       |       |       |
```
The main thread then enters an infinite loop to listen for UDP packets coming on the port specified for the socket.

The main thread and the worker threads all share the same packet queue. This packet queue is essentially a message queue that the main thread uses to communicate with the worker threads.
```
main                           -------------
|       admin shell     queue: | | | | | | |
|-------^                      -------------
|       |
|       |     wrk1    wrk2    wrk3    wrk4
|---------------^-------^-------^-------^
|       |       |       |       |       |
|       |       |       |       |       |
```
When a packet comes in, the main thread takes a mutex lock, so that it knows nobody else is touching the packet queue. Then, it copies the received packet into the packet queue, released the lock, and notifies the other threads that there is something in the packet queue. It does this by calling the `notify_one()` method of something called a std::condition_variable. Then, it loops back to the top to listen for packets again.
```
main                           -------------
|       admin shell     queue: |o| | | | | |
|-------^                      -------------
|       |
|       |     wrk1    wrk2    wrk3    wrk4
|---------------^-------^-------^-------^
|       |       |       |       |       |
|       |       |       |       |       |
```
The worker threads also try to take the mutex lock. But it does this by wrapping it in something special called a std::unique_lock. Creating a unique_lock with the mutex automatically takes out a lock on the mutex it wraps.

Then, the worker thread will wait on the condition_variable shared with main via the method `wait()`. `wait()` will release the mutex wrapped by unique_lock, put the worker thread to sleep, and wait until the main thread wakes it up via the `notify_one()` command.

When the worker thread wakes, it automatically retakes the mutex and has the green light to pull a packet out of the packet queue. Once the worker thread creates a local copy of the packet to work with, it releases the lock, and the packet queue is free to be used. If multiple threads are waiting, only one of them will get woken up by the `notify_one()` command.

```
main                           -------------
|       admin shell     queue: | | | | | | |
|-------^                      -------------
|       |
|       |     wrk1    wrk2    wrk3    wrk4
|---------------^-------^-------^-------^
|       |       |       o       |       |
|       |       |       |       |       |
```
