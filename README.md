Celegraph
==
*Celegraph* is an open-source internet messaging library, written in [C++11][1].

Celegraph was derived from a messaging system developed in [*Oldentide*][2], an open-source multiplayer online role-playing game. Be sure to check it out!

Celegraph aims to provide an easy-to-use and fast messaging library over UDP.


Requirements
------------
*Celegraph* currently supports a Linux development and runtime environment.

Operating Systems
------------
All development and testing is currently done on a 64-bit linux environment. [Ubuntu 16.04.1 LTS][3] is recommended, but other distributions have been known to work, too, including Debian, Fedora, Alpine, and Windows Subsystem for Linux (i.e. WSL or Windows Bash).

Compilers
------------
Celegraph aims to support g++ and clang. The emscripten compiler might be explicitly supported in the future.

Dependencies
------------
* sqlite3 - The sqlite3 command-line tool, used to initialize and manage sqlite databases [sqlite3][5]
* git - Needed to download and install msgpack, catch, and sqlitecpp
* Cmake - Needed to build and compile sqlitecpp and sqlite.
* msgpck-c 2.1.1 - [msgpack-c][6] is used to efficiently and predictably transmit packet data between server and client. This will automatically be installed the first time you run make.
* sqlitecpp 2.2.0 - [SQLiteC++][7] is used to provide an easy-to-use C++ wrapper to the sqlite C API. It comes packaged with a recent version (v3.21) of the sqlite C API library
* Catch 1.10.0 - [Catch][8] is used to run unit and regression tests.


Server and Test Client Usage
------------
In linux, cd to celegraph/ and run

    make

If everything built properly, run

    ./bin/server <Port>

where <Port> is the port the server is using.

To test that the server is running, in a separate terminal, run

    ./bin/client <IP> <Port>

Where <IP> is the address of the server and <Port> is the port of the server.


Docker
------------
To use Docker to run the server, first make sure the docker daemon is running (dockerd).
Then build the image like so:

    docker build -t celegraph_server .

Then create a container instance of the image like so:

    docker run -it -p 1337:1337/udp celegraph_server

Inside the container, run:

    ./bin/server 1337





How Celegraph works - high level:
------------
Currently, there is a single central server that manages everything. All messages between clients are routed through this server.

The server creates a pool of worker threads that each can process a single packet at a time. The main thread listens for packets and forwards those packets to the worker threads via a single packet queue.


How Celegraph works - more details:
------------
When the server is first starts, the main thread spawns an admin shell thread:
```
main
|       admin shell
|-------^
|       |
```
This shell provides the administrator CLI access to the server itself during operation.

Once the admin shell thread has been created, the main thread will then create worker threads (currently, it creates 4):
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
The main thread then listens for UDP packets coming on the port specified for the socket.

The main thread and the worker threads all share the same packet queue. This packet queue is essentially a message queue that the main thread uses to communicate with the worker threads.
```
main                           -------------
|       admin shell     queue: |o|o|o| | | |
|-------^                      -------------
|       |
|       |     wrk1    wrk2    wrk3    wrk4
|---------------^-------^-------^-------^
|       |       |       |       |       |
|       |       |       |       |       |
```
When a packet comes in, the main thread takes a mutex lock, so that it knows nobody else is touching the packet queue. Then, it copies the received packet into the packet queue, released the lock, and notifies the other threads that there is something in the packet queue. It does this by calling the notify_one() method of something called a std::condition_variable. Then, it loops back to the top to listen for packets again.

The worker threads also try to take the mutex lock. But it does this by wrapping it in something special called a std::unique_lock. Creating a unique_lock with the mutex automatically takes out a lock on the mutex it wraps.

Then, the worker thread will wait on the condition_variable shared with main via the method wait(). wait() will release the mutex wrapped by unique_lock, put the worker thread to sleep, and wait until the main thread wakes it up via the notify_one() command.

When the worker thread wakes, it automatically retakes the mutex and has the green light to pull a packet out of the packet queue. Once the worker thread creates a local copy of the packet to work with, it releases the lock, and the packet queue is free to be used. If multiple threads are waiting, only one of them will get woken up by the notify_one() command.




Project TODO:
------------
-Rethink the architecture. Instead of having one single server, what if we make it decentralized/peer-to-peer? What if we make each client capable of being a server?

-Rethink how to do the packets. Maybe reduce packet types? Make it generic?

-Try out my ideas on "levels of abstraction" - Have multiple layers of use of the library

-Create hooks or something into JavsScript/NodeJs?

-Make the client select user to talk to once, instead of each time

-Create a src directory, and separate it from test code

-Use one of the other C message pack implementations that are lighter and faster. This will help reduce compile times.

-Test to make sure that things still work, even if worker threads are created AFTER some packets get put into the queue (does notify_one() have some kind of count, so future wait() calls that aren't already waiting will work? How large is this count?)

-Make sure that packet and user input can't overflow space for strings, to prevent against buffer overflow exploits.

-Look into libraries that can make my life easier, but not add to complexity and compile time

-Create a diagram of how Celegraph works

-Change Main to server_main

-Change things to underscore_case

-Try using DTLS - secure UDP with wolfssl (https://github.com/wolfSSL/wolfssl)
(https://github.com/wolfSSL/wolfssl-examples)



[1]: http://www.cppreference.com/ "C / C++ reference"
[2]: http://www.oldentide.com/ "Oldentide, a game where you can be anyone!"
[3]: http://www.ubuntu.com/ "Ubuntu · The world's most popular free OS"
[4]: https://gcc.gnu.org/ "Gnu C / C++ Compiler"
[5]: https://www.sqlite.org/ "SQLite 3"
[6]: https://github.com/msgpack/msgpack-c/ "msgpack-c"
[7]: https://github.com/SRombauts/SQLiteCpp "SQLiteC++"
[8]: https://github.com/philsquared/Catch "Catch"
