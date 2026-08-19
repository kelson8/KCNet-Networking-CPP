#pragma once

#include <iostream>
#include <string>

class TcpClient {

#ifdef __linux__
public:
    static TcpClient &getInstance()
    {
        static TcpClient instance; // Guaranteed to be destroyed.
        return instance;
    }

    void ConnectToServerSecure();
    int ConnectToServer();

private:
    TcpClient();
    // ~Commands(); // Optional deconstructor
    TcpClient(const TcpClient &) = delete;            // Prevent copy-construction
    TcpClient &operator=(const TcpClient &) = delete; // Prevent assignment

    void SendMessageToServer(int socket, const char* message, size_t messageLength);
#endif // __linux__
};
