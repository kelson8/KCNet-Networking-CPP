#pragma once

#include <iostream>
#include <string>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <arpa/inet.h>

#include "openssl_test.h"

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

class TcpServer
{

#ifdef __linux__
public:
    static TcpServer &getInstance()
    {
        static TcpServer instance; // Guaranteed to be destroyed.
        return instance;
    }

    // Run the secure server, this is not implemented and is broken.
    void RunServerSecure();
    // Run the main TCP server.
    int RunServer();

    // Get the server and client sockets.

    /**
     * Get the server fd value.
     */
    const int GetServerFd() const
    {
        return server_fd;
    }

    /**
     * Get the client fd value.
     */
    const int GetClientFd() const
    {
        return client_fd;
    }

    // Get the users current IP.
    const std::string GetClientIp() const;

    /**
     * Get the current connection packet info
     *
     * TODO Test this.
     */
    const ConnectionPacket GetConnectionPacket() const
    {
        return connectionPacket;
    }

private:
    TcpServer();
    ~TcpServer();                                     // Optional deconstructor
    TcpServer(const TcpServer &) = delete;            // Prevent copy-construction
    TcpServer &operator=(const TcpServer &) = delete; // Prevent assignment

    OpensslTest opensslTest;

    // The public and private key files
    // TODO Make this project use RSA and public/private keys for communication.
    // const char *publicKeyFile;
    // const char *privateKeyFile;

    // Non blocking connections.
    void AcceptAll(int server_fd);
    ssize_t SendNonBlocking(int fd, const char *buf, size_t len);

    // The server and client sockets.
    int server_fd;
    int client_fd;

    sockaddr_in client_address{};

    // The current connection packet, for each user that connects.
    ConnectionPacket connectionPacket;

#ifdef SECURE_SERVER_TEST
    SSL_CTX *ctx;
    SSL *ssl;
#endif // SECURE_SERVER_TEST

    void SetupClientConnection();
    // int SetupClientConnection();
    void ReadFromClient(int client_fd);

#endif // __linux__
};
