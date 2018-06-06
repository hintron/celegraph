---
id: docker
title: How To Use Docker
---

To use Docker to run the server, first make sure the docker daemon is running (dockerd).
Then build the image like so:

    docker build -t celegraph_server .

Then create a container instance of the image like so:

    docker run -it -p 1337:1337/udp celegraph_server

Inside the container, run:

    ./bin/server 1337


