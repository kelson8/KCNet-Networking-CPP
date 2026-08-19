# KCNet-Networking-CPP

This is a basic networking test with C++ and TCP sockets.

Currently, this only supports Linux, Windows support may be added later and I may look into using
the Boost library later for sockets which might be easier.

I have taken this from the KCNet-Linux-Coding internal Git repo that I work on with C and C++ projects.

## Programs
The **TcpClient** folder contains the client that connects to **TcpServer**

The **TcpServer** folder runs a basic TCP socket in a while loop and cleans up with a SIGINT.

## Building
To build this project, you will require the below dependencies

```bash
sudo apt install cmake python3-dev libicu-dev libbz2-dev zlib1g-dev
```

To build this, you can run the `clean-build.sh` script which deletes the build folder and recreates it.

## Preprocessors
You can enable/disable some preprocessors and change their values in the `defines.h` files within the projects.

I have the `SECURE_SERVER_TEST` preprocessor that is currently broken, it is supposed to enable the secure server but breaks it with my new setup.

## Usage
What the client does, is uses `/dev/urandom`, in `TcpClient` under the `TcpClient::ConnectToServer()` function, which calls `Urandom::GenerateRandom` in the `urandom_test.cpp` file.

The server just listens for the messages sent by the client or anything, currently there is no working security or SSL
so this just sends all data in plain text.

## Running

To run the projects on their own:
* `./run-projects.sh TcpServer` - To start the server.
* `./run-projects.sh TcpClient` - To connect to the server and send a message from the code.

To test the TCP server messaging
* Run `./test-tcp.sh`

I will make a Dockerfile to build and test this project later.

# Libraries
This project is using the base64 library which can be found with the below link.
* https://github.com/tobiaslocker/base64

# License
I have decided to license this project under the GPLv3 license.
