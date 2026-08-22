#include "openssl_test.h"

#ifdef __linux__
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>

#include <fstream>
#include <sstream>

#include <memory>
#include <string>

#ifdef NEW_SERVER_TEST
#include "TCPClient.h"
#include "TCPServer.h"
#include "TCPSSLServer.h"
#include "TCPSSLClient.h"
#endif // NEW_SERVER_TEST

// #include "decrypt.h"
// #include "encrypt.h"
// #include "keyPair.h"

#include "base64.hpp"
#include "file_functions.h"

// TODO Move this into a shared library between TcpServer and TcpClient.
// TODO Look into using this library for this, it is a C library but I can still use it.
// https://github.com/libtom/libtomcrypt

// New packages required for libtomcrypt
// libtommath-dev libtommath1

//---------
// TCP Client
//---------

// Here is the guide I used for this basic networking test, which seems to work well.
// https://codezup.com/c-networking-guide/

/**
 * Init Openssl
 */
void OpensslTest::init_openssl(void)
{
    SSL_library_init();           // Initialize core library
    OpenSSL_add_all_algorithms(); // Load crypto algorithms (e.g., RSA)
    SSL_load_error_strings();     // Load error messages
}

// https://linuxvox.com/blog/turn-a-simple-socket-into-an-ssl-socket/

/**
 * Create the TCP Client
 */
int OpensslTest::create_tcp_client(const char *server_ip, int port)
{
    // Create the socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    //

    // Check if the server IP is valid.
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
    {
        perror("invalid server IP");
        close(sockfd);
        return -1;
        // exit(EXIT_FAILURE);
    }
    //

    // Attempt to connect to the server.
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connection failed");
        close(sockfd);
        return -1;
        // exit(EXIT_FAILURE);
    }

    //
    return sockfd;
}

/**
 * Create the SSL context for TLS
 */
SSL_CTX *OpensslTest::create_client_ssl_context()
{
    const SSL_METHOD *method = TLS_client_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (!ctx)
    {
        perror("client SSL context failed");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return ctx;
}
//



#endif // __linux__