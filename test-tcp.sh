#!/bin/bash

# Basic script to start my TcpClient and TcpServer programs.

# The amount of time to wait before running the client to send a test message.
WAIT_TIME=2
MESSAGE_TO_SEND="Hello from KCNet-Networking-CPP (TCP)"

# https://www.geeksforgeeks.org/linux-unix/bash-scripting-functions/
# Basic function to print an error message.
# Appends 'Error:' to the message.
print_error() {
    echo "Error: $1"
}

# Programs
TCP_SERVER="./tcp-server"
TCP_CLIENT="./tcp-client"

# Make sure the build folder exists.
if [ ! -d "./build" ]; then
    print_error "build folder not found, run ./clean-build.sh first."
    exit 1
fi

# Switch to the build folder
echo "Switching to build folder"
cd ./build || exit 1

# Store the build folder to switch back into.
CWD=$(pwd)

if [ ! -f "./TcpServer/$TCP_SERVER" ] || [ ! -f "./TcpClient/$TCP_CLIENT" ]; then
    echo "The TcpServer or TcpClient programs do not exist! Build the project first."
    exit 1
fi

# Set the name of the screen session here for the server.
SCREEN_NAME="TcpServer"

# Run the programs directly.
# No wonder I couldn't stop this, I was running the server forever with the '&'.
# Since it had no way to kill the process it would just stay open forever.

# https://unix.stackexchange.com/questions/162133/run-script-in-a-screen
# I switched to using screen for the server
# screen -dmS "$SCREEN_NAME" ./build/TcpServer/tcp-server

# Change to the tcp server folder
cd "TcpServer" || exit 1
# Run the screen session
screen -dmS "$SCREEN_NAME" "$TCP_SERVER"

sleep "$WAIT_TIME"
# "build/TcpClient/tcp-client" "$MESSAGE_TO_SEND" &

# Change back to the tcp client folder.
cd "$CWD/TcpClient" || exit 1
# Currently the message sending is disabled, so this sends output from /dev/urandom to the server.
"$TCP_CLIENT" "$MESSAGE_TO_SEND" &

# Kill the screen session
# https://askubuntu.com/questions/356006/kill-a-screen-session
screen -XS "$SCREEN_NAME" quit
