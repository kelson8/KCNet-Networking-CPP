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
#include <fcntl.h>

#include <string>

#include "base64.hpp"
#include "defines.h"

// TODO Move this into a shared library between TcpServer and TcpClient.

//---------
// TCP Server
//---------

/**
 * Init Openssl
 */
void OpensslTest::init_openssl(void)
{
    SSL_library_init();           // Initialize core library
    OpenSSL_add_all_algorithms(); // Load crypto algorithms (e.g., RSA)
    SSL_load_error_strings();     // Load error messages
}

/**
 * Create the TCP Socket
 */
int OpensslTest::create_tcp_socket(int port)
{

    // Adding SOCK_NONBLOCK to this should allow this to be non-blocking.
    // Well this just spams it with 'accept: bad file descriptor'
    // https://stackoverflow.com/a/22339017

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // int server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    // Set the socket as non-blocking
    // TODO Test this
    // Well this just breaks the server.
    // https://beej.us/guide/bgnet/html/#blocking
    // fcntl(server_fd, F_SETFL, O_NONBLOCK);
    // Set the socket as async.
    // fcntl(server_fd, F_SETFL, O_ASYNC);

    // New for non blocking test
    // https://linuxvox.com/blog/simultaneously-read-and-write-on-the-same-socket-in-c-or-c/

    if (server_fd < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Lets the port be reused immediately after this process exits,
    // otherwise a quick restart fails with "Address already in use".
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to port
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for connections (max 5 pending)
    if (listen(server_fd, 5) < 0)
    {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // printf("TCP server listening on port %d...\n", LISTEN_PORT);
    // log_output("TCP server listening on port " + LISTEN_PORT);
    log_output("TCP server listening on port ", LISTEN_PORT);
    return server_fd;
}

/**
 * Cleanup Openssl
 */
void OpensslTest::openssl_cleanup(void)
{
    EVP_cleanup();
}

/**
 * Create the SSL context for TLS
 */
SSL_CTX *OpensslTest::create_ssl_context()
{
    const SSL_METHOD *method = TLS_server_method(); // Use modern TLS
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (!ctx)
    {
        perror("SSL context creation failed");
        ERR_print_errors_fp(stderr); // Print OpenSSL errors
        exit(EXIT_FAILURE);
    }
    return ctx;
}

/**
 * Configure the server context with certificates and options.
 *
 * @param ctx The
 * @param publicKeyFile The public key file to use for the server.
 * @param privateKeyFile The private key file for the server.
 */
// void configure_ssl_context(SSL_CTX* ctx) {
void OpensslTest::configure_ssl_context(SSL_CTX *ctx, const char *publicKeyFile, const char *privateKeyFile)
{
    // Load server certificate (PEM format)
    if (SSL_CTX_use_certificate_file(ctx, publicKeyFile, SSL_FILETYPE_PEM) <= 0)
    {
        log_output("Error: public key could not be found");
        // ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    // Load private key (PEM format)
    if (SSL_CTX_use_PrivateKey_file(ctx, privateKeyFile, SSL_FILETYPE_PEM) <= 0)
    {
        log_output("Error: private key could not be found");
        // ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    // Verify private key matches certificate
    if (!SSL_CTX_check_private_key(ctx))
    {
        log_output("Error: Private key does not match certificate!");
        // fprintf(stderr, "Private key does not match certificate!\n");
        exit(EXIT_FAILURE);
    }
}

#endif // __linux__
