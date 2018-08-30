Celegraph
==
*Celegraph* is an open-source messaging library, written in [C++11][1].

[![Build Status](https://travis-ci.org/hintron/celegraph.svg?branch=master)](https://travis-ci.org/hintron/celegraph)

Celegraph was derived from a messaging system developed in [*Oldentide*][2], an in-development open-source massive multiplayer online role-playing game. Be sure to check it out!



Quickstart
--------------

Install build tools:

    sudo apt install build-essential meson cmake

Build 3rd-party dependencies:

    make deps

Build:

    meson build
    cd build
    ninja

Install:

    sudo ninja install

Run Server:

    celegraph-server <port>

Run Client:

    celegraph-client <addr> <port>

Uninstall:

    sudo ninja uninstall


[1]: http://www.cppreference.com/ "C / C++ reference"
[2]: http://www.oldentide.com/ "Oldentide, a game where you can be anyone!"
