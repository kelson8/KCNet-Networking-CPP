cmake_minimum_required(VERSION 3.22)
project(KCNet-Networking-CPP)

# This works for a central CMakeLists file for my projects.

# This should include the lib and the headers in each project that is included in this CMake with add_subdirectory.

include(FetchContent)

#---------
# Tomlplusplus
# TODO Use this later.
#---------

FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.4.0
)

FetchContent_MakeAvailable(tomlplusplus)

#---------
# Setup Base64
#---------
add_subdirectory(lib/base64)

#### Not in use
# add_subdirectory(lib/socket-cpp)

# add_subdirectory(lib/RSA)

# TinyRSA
# add_subdirectory(lib/TinyRSA)

# Bigint
# https://github.com/rgroshanrg/bigint/tree/main
# Moved into lib/TinyRSA/lib
# add_subdirectory(lib/bigint)

#####

#--------
# Projects
#--------


# Tcp client and server.
add_subdirectory(TcpClient)

add_subdirectory(TcpServer)

