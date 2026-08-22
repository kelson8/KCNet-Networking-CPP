#pragma once

#include <iostream>
#include <string>

enum ServerCommand
{
    NONE = -1,
    URANDOM_MESSAGE,
    RELOAD_COMMAND,
    SHUTDOWN_COMMAND,

    // This should always be last in the command list.
    LAST_COMMAND,
};

class TcpClient
{

#ifdef __linux__
public:
    static TcpClient &getInstance()
    {
        static TcpClient instance; // Guaranteed to be destroyed.
        return instance;
    }

    void ConnectToServerSecure();
    // int ConnectToServer();
    int ConnectToServer(ServerCommand command);

private:
    TcpClient();
    // ~Commands(); // Optional deconstructor
    TcpClient(const TcpClient &) = delete;            // Prevent copy-construction
    TcpClient &operator=(const TcpClient &) = delete; // Prevent assignment

    void SendMessageToServer(int socket, const char *message, size_t messageLength);
#endif // __linux__
};
