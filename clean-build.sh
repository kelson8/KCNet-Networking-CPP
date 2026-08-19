#!/bin/bash

# Script to remove the build directory and re-create it.
# Run cmake for build
# Run make to build program
# Run the program if it exists

if [ -d ./build ]; then
    rm -rf build
    echo "Removed build directory"
fi

if [ ! -d ./build ]; then
    mkdir build || exit
    echo "Re-created build directory"
fi

# cmake .. 
# Fix for a compiler error with fmt
# https://stackoverflow.com/questions/77758069/relocation-r-x86-64-pc32-against-symbol-stderrglibc-2-2-5-can-not-be-used-wh
cmake -DBUILD_SHARED_LIBS=ON -B build
echo "CMake files generated"

cmake --build build --parallel

# make -j"$(nproc)"
