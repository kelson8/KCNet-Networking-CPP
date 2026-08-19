#!/bin/bash

# This script will just run the make command for the project.
# clean-build.sh will need to be run first for CMake.

# This will be generated with cmake
MAKEFILE="./build/Makefile"

# https://www.geeksforgeeks.org/linux-unix/bash-scripting-functions/
# Basic function to print an error message.
# Appends 'Error:' to the message.
print_error() {
    echo "Error: $1"
}

if [ ! -d "./build" ]; then
    print_error "build folder not found, run ./clean-build.sh first."
    exit 1
fi

if [ -f "$MAKEFILE" ]; then
    cd ./build || exit 1
    make -j"$(nproc)"
    cd ..
else
    print_error "$MAKEFILE doesn't exist"
    exit 1
fi
