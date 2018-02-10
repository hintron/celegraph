Oldentide Dedicated Server
==
The *Oldentide Dedicated Server* is an open source project, built in [C++][1], to implement the
backend game state for the multiplayer online role-playing game [*Oldentide*][2].  This
directory contains all of the code necessary to build and run the dedicated server.

The "from scratch" build process breaks down into three main steps, with an additional
step used during the development process:

1. Create the database and generate all of the needed tables.
2. Populate the database with NPC information.
3. Compile the dedicated server.
4. Compile the test client.

Requirements
------------
The *Oldentide Dedicated Server* currently supports a Linux development and runtime environment.

Operating Systems
------------
All development and testing is currently done on a 64-Bit linux environment.
[Ubuntu 16.04.1 LTS][3] is recommended, but other distributions have been known to work,
too, including Debian, Fedora, Alpine, and Windows Bash.

Compilers
------------
Building *Oldentide Dedicated Server* is currently possible with the following compiler:
* *g++* - Currently all development is done on the [g++][4] compiler in a 64-bit
   environment.  A makefile is included to simplify compilation!

*cmake may possibly be supported in future releases...*

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
In linux, cd into Oldentide/Server/ and run

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




[1]: http://www.cppreference.com/ "C / C++ reference"
[2]: http://www.oldentide.com/ "Oldentide, a game where you can be anyone!"
[3]: http://www.ubuntu.com/ "Ubuntu · The world's most popular free OS"
[4]: https://gcc.gnu.org/ "Gnu C / C++ Compiler"
[5]: https://www.sqlite.org/ "SQLite 3"
[6]: https://github.com/msgpack/msgpack-c/ "msgpack-c"
[7]: https://github.com/SRombauts/SQLiteCpp "SQLiteC++"
[8]: https://github.com/philsquared/Catch "Catch"
