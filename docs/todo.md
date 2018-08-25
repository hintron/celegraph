---
id: todo
title: Project TODO
---

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

-Try using DTLS - secure UDP with [wolfssl](https://github.com/wolfSSL/wolfssl). see (https://github.com/wolfSSL/wolfssl-examples)

-Modify .travis.yml to use meson