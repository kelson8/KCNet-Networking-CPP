#!/bin/bash

# Basic script to start my TcpClient and TcpServer programs.
# To run this:

# Running Client
# ./run-projects.sh TcpClient

# Running server
# ./run-projects.sh TcpServer

# Program list:
# TcpClient
# TcpServer

BUILD_FOLDER=build

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <program>"
    echo "Program list: TcpClient, TcpServer"
    exit 1
fi

PROGRAM_TO_RUN="$1"
# PROGRAM="$BUILD_FOLDER/$PROGRAM_TO_RUN"

# Make sure the program folder exists first.
if [ ! -d "$BUILD_FOLDER/$PROGRAM_TO_RUN" ]; then
    echo "$BUILD_FOLDER/$PROGRAM_TO_RUN doesn't exist." 
fi

# Run whichever program was selected.
# Now this switches to the build folder directory and doesn't use the 'Program' variable above.
if [ "$PROGRAM_TO_RUN" == "TcpServer" ]; then
    cd "build/TcpServer" || exit
    "./tcp-server"
    cd .. || exit
elif [ "$PROGRAM_TO_RUN" == "TcpClient" ]; then
    cd "build/TcpClient" || exit
    "./tcp-client"
    cd .. || exit
fi
