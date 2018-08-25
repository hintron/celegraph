Celegraph
==
*Celegraph* is an open-source messaging library, written in [C++11][1].

[![Build Status](https://travis-ci.org/hintron/celegraph.svg?branch=master)](https://travis-ci.org/hintron/celegraph)

Celegraph was derived from a messaging system developed in [*Oldentide*][2], an in-development open-source massive multiplayer online role-playing game. Be sure to check it out!

Configuring, building, and installing with Meson
------------

Build dependencies:

    make sqlitecpp
    make msgpack
    make catch

Build:

    mkdir build
    meson build
    cd build
    ninja

Install:

    sudo ninja install

Uninstall:

    sudo ninja uninstall


Server and Test Client Usage
------------

    celegraph-server <port>

where <port> is the port the server is using.

To test that the server is running, in a separate terminal, run

    celegraph-client <addr> <port>

Where <addr> is the address of the server and <port> is the port of the server.


[1]: http://www.cppreference.com/ "C / C++ reference"
[2]: http://www.oldentide.com/ "Oldentide, a game where you can be anyone!"
