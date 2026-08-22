# KCNet-Networking-CPP

This is a basic networking test with C++ and TCP sockets.

Currently, this only supports Linux, Windows support may be added later and I may look into using
the Boost library later for sockets which might be easier.

I have taken this from the KCNet-Linux-Coding internal Git repo that I work on with C and C++ projects.

## Programs
The **TcpClient** folder contains the client that connects to **TcpServer**

The **TcpServer** folder runs a basic TCP socket in a while loop and cleans up with a SIGINT.

## Building
To build this project, you will require the below dependencies, I will need to write down a full list later.

```bash
sudo apt install cmake python3-dev libicu-dev libbz2-dev zlib1g-dev screen
```

To build this, you can run the `clean-build.sh` script which deletes the build folder and recreates it.

If you have already built this with CMake, run the `build.sh` script to rebuild without deleting the build folder.



### Windows building

TODO Fix this guide for Windows install, currently this doesn't do anything on Windows yet.
To build this on Windows, you will require OpenSSL to be installed.

* https://tecadmin.net/install-openssl-on-windows/
* https://slproweb.com/products/Win32OpenSSL.html

Currently, this project won't build on Linux, but I figured out how to install OpenSSL for my needs.

I finally got OpenSSL working on Windows with this GitHub repo
* https://github.com/chronoxor/OpenSSL

To setup openssl on Windows

1. Download and setup msys2 from here, I use the `MSYS2 MSYS executable` 
https://www.msys2.org/

2. Open up the `MSYS2 MSYS` executable.

3. Update msys2 package list
```bash
pacman -Syyu
```

4. Install all build packages
```bash
pacman -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-gcc mingw-w64-x86_64-openssl mingw-w64-x86_64-git-perl gcc
```

5. Download and setup OpenSSL from here
* https://slproweb.com/products/Win32OpenSSL.html

6. Download the latest version of this from here for MinGW and extract the contents of the file.
* https://github.com/chronoxor/OpenSSL

7. Copy the MinGW/lib64 folder contents from the extracted file into `C:\Program Files\OpenSSL-Win64\lib\MinGW`.

8. Setup the Environment path variables in Windows.

```
OPENSSL_INCLUDE_DIR
C:\Program Files\OpenSSL-Win64\include

OPENSSL_LIBRARIES
C:\Program Files\OpenSSL-Win64\lib\MinGW

OPENSSL_ROOT_DIR
C:\Program Files\OpenSSL-Win64
```

I will need to setup TCP sockets in Windows before I can make this client also work on Windows.

## Preprocessors
You can enable/disable some preprocessors and change their values in the `defines.h` files within the projects.

I have the `SECURE_SERVER_TEST` preprocessor that is currently broken, it is supposed to enable the secure server but breaks it with my new setup.

## Usage
What the client does, is uses `/dev/urandom`, in `TcpClient` under the `TcpClient::ConnectToServer()` function, which calls `Urandom::GenerateRandom()` in the `urandom_test.cpp` file.

The server just listens for the messages sent by the client or anything, currently there is no working security or SSL
so this just sends all data in plain text.

## Running

**Without Docker**

To run the projects on their own
* `./run-projects.sh TcpServer` - To start the server.
* `./run-projects.sh TcpClient` - To connect to the server and send a message from the code.

To test the TCP server messaging, now runs with `screen` on Linux.
* Run `./test-tcp.sh`

**With Docker**

To build this with Docker
```bash
docker build -t kcnet-networking-cpp:0.0.1a .
```

You can run the test server with docker compose, this runs the tcp-server for testing, and the client can send text or commands to this server.
```bash
# Run this to start the test server and be able to close it easily.
docker compose up
```

## GitHub Actions
To setup the GitHub actions for the runner of this project if forking the project

This is only required for the Gitea workflow, the GitHub one doesn't need these.

Set these values in the actions enviornment
* GIT_REPO - Set this to your Git repo path, example for GitHub https://github.com/kelson8/KCNet-Networking-CPP.git
* REPO_AUTHOR - Set this to your GitHub name

# Libraries
Here is a list of libraries that this project is using

| Library Name | URL | Description |
| ----- | ------ | ------ |
| Argh | https://github.com/adishavit/argh | This is for command argument handling. |
| Base64 | https://github.com/tobiaslocker/base64 | For base64 encoding/decoding |
| Fmt | https://github.com/fmtlib/fmt | For formatting text easier with C++ |
| OpenSSL | https://openssl-library.org/source/  | This is in use for the TcpClient and TcpServer. |
| OpenSSL Win32 | https://slproweb.com/products/Win32OpenSSL.html | This is used for building with Windows. |
| Spdlog | https://github.com/gabime/spdlog | Makes logging to files much easier. |
| tomlplusplus | https://github.com/marzer/tomlplusplus | For toml config handling, this isn't currently in use yet. |

# License
I have decided to license this project under the GPLv3 license.
