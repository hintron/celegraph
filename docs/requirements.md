---
id: requirements
title: Requirements
---

*Celegraph* currently supports a Linux development and runtime environment.

## Operating Systems

All development and testing is currently done on a 64-bit linux environment. [Ubuntu 16.04.1 LTS][3] is recommended, but other distributions have been known to work, too, including Debian, Fedora, Alpine, and Windows Subsystem for Linux (i.e. WSL or Windows Bash).

## Compilers

Celegraph aims to support g++ and clang. The emscripten compiler might be explicitly supported in the future.

## Dependencies

* **sqlite3** - The sqlite3 command-line tool, used to initialize and manage sqlite databases [sqlite3][5]
* **git** - Needed to download and install msgpack, catch, and sqlitecpp
* **Cmake** - Needed to build and compile sqlitecpp and sqlite.
* **msgpck-c 2.1.1** - [msgpack-c][6] is used to efficiently and predictably transmit packet data between server and client. This will automatically be installed the first time you run make.
* **sqlitecpp 2.2.0** - [SQLiteC++][7] is used to provide an easy-to-use C++ wrapper to the sqlite C API. It comes packaged with a recent version (v3.21) of the sqlite C API library
* **Catch 1.10.0** - [Catch][8] is used to run unit and regression tests.

[1]: http://www.cppreference.com/ "C / C++ reference"
[2]: http://www.oldentide.com/ "Oldentide, a game where you can be anyone!"
[3]: http://www.ubuntu.com/ "Ubuntu · The world's most popular free OS"
[4]: https://gcc.gnu.org/ "Gnu C / C++ Compiler"
[5]: https://www.sqlite.org/ "SQLite 3"
[6]: https://github.com/msgpack/msgpack-c/ "msgpack-c"
[7]: https://github.com/SRombauts/SQLiteCpp "SQLiteC++"
[8]: https://github.com/philsquared/Catch "Catch"
