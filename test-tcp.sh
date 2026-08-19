#!/bin/bash

# Basic script to start my TcpClient and TcpServer programs.

# The amount of time to wait before running the client to send a test message.
WAIT_TIME=2
MESSAGE_TO_SEND="Hello from KCNet-Networking-CPP (TCP)"

# Run the programs directly.
"build/TcpServer/tcp-server" &

sleep "$WAIT_TIME"
# "build/TcpClient/tcp-client" &
"build/TcpClient/tcp-client" "$MESSAGE_TO_SEND" &
