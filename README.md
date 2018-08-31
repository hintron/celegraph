Celegraph
==
*Celegraph* is an open-source messaging library, written in [C++11][1].

[![Build Status](https://travis-ci.org/hintron/celegraph.svg?branch=master)](https://travis-ci.org/hintron/celegraph)

Celegraph was derived from a messaging system developed in [*Oldentide*][2], an in-development open-source massive multiplayer online role-playing game. Be sure to check it out!



Quickstart
--------------

Install required packages:

    sudo apt install build-essential ninja-build python3 python3-setuptools sqlite3 cmake

Install the latest version of Meson:

    pip3 install meson

Build 3rd-party dependencies:

    make deps

Build:

    meson build
    cd build
    ninja

Install:

    sudo ninja install

Run Server:

    sudo celegraph-server <port>

Run Client:

    celegraph-client <addr> <port>

Uninstall:

    sudo ninja uninstall


[1]: http://www.cppreference.com/ "C / C++ reference"
[2]: http://www.oldentide.com/ "Oldentide, a game where you can be anyone!"
