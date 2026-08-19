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
    mkdir build && cd build || exit
    echo "Re-created build directory"
else
    # Just change to build folder if it already exists.
    # If this fails, it should exit instead of continuing
    cd build || exit
fi

cmake ..
echo "CMake files generated"

make -j"$(nproc)"
