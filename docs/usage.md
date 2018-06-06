---
id: usage
title: Server and Test Client Usage
---

In linux, cd to celegraph/ and run

    make

If everything built properly, run

    ./bin/server <Port>

where <Port> is the port the server is using.

To test that the server is running, in a separate terminal, run

    ./bin/client <IP> <Port>

Where <IP> is the address of the server and <Port> is the port of the server.