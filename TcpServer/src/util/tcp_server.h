#pragma once

#include <iostream>
#include <string>


#include <openssl/ssl.h>
#include <openssl/err.h>


#include "openssl_test.h"

class TcpServer {

#ifdef __linux__
public:
    static TcpServer &getInstance()
    {
        static TcpServer instance; // Guaranteed to be destroyed.
        return instance;
    }

    void RunServerSecure();
    int RunServer();

    const int GetServerFd() const;
    const int GetClientFd() const;


private:
    TcpServer();
    ~TcpServer(); // Optional deconstructor
    TcpServer(const TcpServer &) = delete;            // Prevent copy-construction
    TcpServer &operator=(const TcpServer &) = delete; // Prevent assignment

    OpensslTest opensslTest;
    
    // const char *publicKeyFile;
    // const char *privateKeyFile;

    int server_fd;
    int client_fd;

#ifdef SECURE_SERVER_TEST
    SSL_CTX *ctx;
    SSL *ssl;
#endif // SECURE_SERVER_TEST

    void SetupClientConnection();
    // int SetupClientConnection();
    void ReadFromClient(int client_fd);

#endif // __linux__

};

