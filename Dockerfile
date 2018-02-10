# Oldentide Docker File

# How To Use:

# make sure the docker daemon is running (dockerd)
# cd into this directory and run:
#   docker build -t oldentide_server .
#   docker run -it -p 1337:1337/udp oldentide_server
#   ./bin/Server 1337
# This will map the docker container to localhost's 1337 port
# Now access it via another shell like so:
#   ./bin/Client 127.0.0.1 1337

# Base image
FROM alpine:3.6

# Set the working directory
WORKDIR /oldentide

# Copy the current directory contents into the container at the working directory
ADD . /oldentide

# --no-cache is like --update, but deletes package list cache automatically
# To search for valid packages, go to https://pkgs.alpinelinux.org/packages
RUN apk --no-cache add \
    bash \
    cmake \
    make\
    g++ \
    git \
    sqlite

RUN ["make"]

# Make ports available to the world outside this container
# UDP ports need to be explicitly specified
EXPOSE 1337/udp

# Spawn a command line interface
CMD ["bash"]