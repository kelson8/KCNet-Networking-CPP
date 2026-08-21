#pragma once

#include <iostream>
#include <string>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <arpa/inet.h>

#include "openssl_test.h"

class TcpServer
{

#ifdef __linux__
public:
    /**
     * Connection packet struct, this will store the users IP and if they are banned.
     * TODO Use this later.
     * TODO Make this a required option to connect to the TCP server, otherwise refuse connections.
     * These options will need to be validated on the server.
     */
    typedef struct ConnectionPacket
    {
        // The users IP
        std::string userIp;
        // The users name, not implemented yet.
        // std::string userName;
        // Is the user banned?
        bool isUserBanned;
        // Does the client support console colors?
        // bool doesSupportConsoleColors;
    } ConnectionPacket;

    static TcpServer &getInstance()
    {
        static TcpServer instance; // Guaranteed to be destroyed.
        return instance;
    }

    void RunServerSecure();
    int RunServer();

    const int GetServerFd() const;
    const int GetClientFd() const;

    const std::string GetClientIp() const;

private:
    TcpServer();
    ~TcpServer();                                     // Optional deconstructor
    TcpServer(const TcpServer &) = delete;            // Prevent copy-construction
    TcpServer &operator=(const TcpServer &) = delete; // Prevent assignment

    OpensslTest opensslTest;

    // const char *publicKeyFile;
    // const char *privateKeyFile;

    void AcceptAll(int server_fd);
    ssize_t SendNonBlocking(int fd, const char *buf, size_t len);

    int server_fd;
    int client_fd;

    sockaddr_in client_address{};

#ifdef SECURE_SERVER_TEST
    SSL_CTX *ctx;
    SSL *ssl;
#endif // SECURE_SERVER_TEST

    void SetupClientConnection();
    // int SetupClientConnection();
    void ReadFromClient(int client_fd);

#endif // __linux__
};
