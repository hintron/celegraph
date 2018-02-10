Celegraph
==
*Celegraph* is an open-source internet messaging library, written in [C++][1].

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

    bin/Server <Port>

where <Port> is the port the server is using.

To test that the server is running, in a separate terminal, run

    bin/Client <IP> <Port>

Where <IP> is the address of the server and <Port> is the port of the server.


Docker
------------
To use Docker to run the server, first make sure the docker daemon is running (dockerd).
Then build the image like so:

    docker build -t oldentide_server .

Then create a container instance of the image like so:

    docker run -it -p 1337:1337/udp oldentide_server

Inside the container, run:

    ./bin/Server 1337


Project TODO:
------------
-Get rid of Oldentide vestiges

-Make the client select user to talk to once, instead of each time

-Create a src directory, and separate it from test code

-Use one of the other C message pack implementations that are lighter and faster. This will help reduce compile times.

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
