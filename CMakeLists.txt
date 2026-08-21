cmake_minimum_required(VERSION 3.22)
project(KCNet-Networking-CPP)

# This works for a central CMakeLists file for my projects.

# This should include the lib and the headers in each project that is included in this CMake with add_subdirectory.

include(FetchContent)

#-------------
# Fmt
#-------------

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        12.2.0
  GIT_PROGRESS   TRUE
  GIT_SHALLOW    TRUE
)

# This is required to be set for fmt being here for certain builds.
set(OLD_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

FetchContent_MakeAvailable(fmt)

# Reset the flags back to normal.
set(CMAKE_CXX_FLAGS "${OLD_CMAKE_CXX_FLAGS}")

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

