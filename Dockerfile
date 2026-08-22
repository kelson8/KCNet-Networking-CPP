# The new base image to contain runtime dependencies

# I got this to build in docker
# To build this, run: docker build -t kcnet-networking-cpp:0.0.1a .
# To run and view build output: 
# docker run --rm -it kcnet-networking-cpp:0.0.1a /bin/bash
# ls ./bin

# https://www.jmoisio.eu/en/blog/2020/06/01/building-cpp-containers-using-docker-and-cmake/

# Using this docker file
# I modified this Dockerfile a bit
# https://raw.githubusercontent.com/StefanoLusardi/docker_cpp/refs/heads/main/Dockerfile

# Removed: ninja-build, python and conan

ARG UBUNTU_VERSION=24.04

#----------
# Base stage
#----------
FROM ubuntu:${UBUNTU_VERSION} AS base

ARG C_COMPILER=gcc
ARG CXX_COMPILER=g++
# Switch compiler version to 14
# https://documentation.ubuntu.com/ubuntu-for-developers/reference/availability/gcc/
# ARG COMPILER_VERSION=10
# ARG COMPILER_VERSION=12
ARG COMPILER_VERSION=14

# The wxwidgets version to install, 3.2 is on Ubuntu 24.04.
ENV WXGTK3_VERSION="3.2"

# Removed: python3.10 python3-pip ninja-build \

# https://dev.to/ajeetraina/how-to-clear-docker-cache-2nnp

RUN apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
    make cmake \
    ca-certificates git \
    libsqlite3-dev \
    libssl-dev \
    libwxgtk${WXGTK3_VERSION}-dev \
    ${C_COMPILER}-${COMPILER_VERSION} \
    ${CXX_COMPILER}-${COMPILER_VERSION} && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

#----------
# Build stage
#----------
FROM base AS build

# Set the C and C++ compiler.
ENV CC=${C_COMPILER}-${COMPILER_VERSION}
ENV CXX=${CXX_COMPILER}-${COMPILER_VERSION}

# Set this as the build type
# Can either be Debug or Release
ARG BUILD_TYPE=Release

ENV BUILD_TYPE=${BUILD_TYPE}

# Change the workdir and copy files
WORKDIR /KCNet-Networking-CPP

# Copy only the necessary files for the first part to cache git submodules
COPY .git .git
COPY CMakeLists.txt CMakeLists.txt

# Setup the git submodules, without these the project won't function
RUN git submodule update --init --recursive

# # Copy in the rest of the files
COPY . .
###

# TODO Figure out how to cache the build stage.
# Build with the specified repo build type.
RUN mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} .. && make -j"$(nproc)"

#----------
# Cache Stage, copy files from build output to the cache.
#----------
FROM ubuntu:${UBUNTU_VERSION} AS cache
WORKDIR /KCNet-Networking-CPP
COPY --from=build /KCNet-Networking-CPP/build/TcpClient/tcp-client /KCNet-Networking-CPP/bin/tcp-client
COPY --from=build /KCNet-Networking-CPP/build/TcpServer/tcp-server /KCNet-Networking-CPP/bin/tcp-server
#

#----------
# Publish stage, project should be built.
#----------
FROM ubuntu:${UBUNTU_VERSION} AS publish

WORKDIR /KCNet-Networking-CPP

COPY --from=cache /KCNet-Networking-CPP/bin /KCNet-Networking-CPP/bin
