

#ifdef __linux__
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <filesystem>

#include <fcntl.h>

// #include <cryptopp/base64.h>
// #include <cryptopp/filters.h>

// TODO Look into this later.
// https://github.com/embeddedmz/socket-cpp

// TODO Look into this for packets
// https://github.com/taerugh/can-cpp/blob/main/can/src/packet.cpp

#include "tcp_server.h"

#include "defines.h"
#include "openssl_test.h"
#include "file_functions.h"
#include "toml_handler.h"

#include "logger.h"

// #include "coroutine_test.h"

//--------------
// Server for TCP testing.
//--------------

TcpServer::TcpServer()
{
#ifdef SECURE_SERVER_TEST
    log_output("(SECURE) Starting TCP socket with SSL for TcpServer.");
    Logger::getInstance().Log(LogLevel::LOG_INFO, "(SECURE) Starting TCP socket with SSL for TcpServer.");
#else
    log_output("(INSECURE) Starting TCP socket without SSL for TcpServer.");
    Logger::getInstance().Log(LogLevel::LOG_INFO, "(INSECURE) Starting TCP socket without SSL for TcpServer.");
#endif // SECURE_SERVER_TEST

#ifdef SECURE_SERVER_TEST
    server_fd = opensslTest.create_tcp_socket(LISTEN_PORT);

    // Create and configure SSL context
    ctx = opensslTest.create_ssl_context();

    // Configure the SSL keys and context.
    // const char *publicKeyFile = PUBLIC_KEY;
    // publicKeyFile = PUBLIC_KEY;
    // // const char *privateKeyFile = PRIVATE_KEY;
    // privateKeyFile = PRIVATE_KEY;
    // opensslTest.configure_ssl_context(ctx, publicKeyFile, privateKeyFile);

    // Create SSL object for this connection
    ssl = SSL_new(ctx);

    // Associate SSL object with client socket
    SSL_set_fd(ssl, client_fd);

#else
    server_fd = opensslTest.create_tcp_socket(LISTEN_PORT);
    // 
    // Non blocking test
    // https://stackoverflow.com/questions/1543466/how-do-i-change-a-tcp-socket-to-be-non-blocking
    // int status = fcntl(server_fd, F_SETFL, fcntl(server_fd, F_GETFL, 0) | O_NONBLOCK);

    // if (status == -1){
    //     perror("calling fcntl");
    //     // handle the error.  By the way, I've never seen fcntl fail in this way
    // }
    // //
#endif // SECURE_SERVER_TEST
}

TcpServer::~TcpServer()
{
    log_output("Cleaning up and shutting down TcpServer.");
    Logger::getInstance().Log(LogLevel::LOG_INFO, "Cleaning up and shutting down TcpServer.");
    // Close connections and set values back to 0.
    close(client_fd); // Close TCP socket
    close(server_fd); // Close listening socket

    server_fd = 0;
    client_fd = 0;

// TODO Move this here
#ifdef SECURE_SERVER_TEST
    // Cleanup
    SSL_shutdown(ssl); // Gracefully close SSL session
    SSL_free(ssl);     // Free SSL object
    SSL_CTX_free(ctx); // Free SSL context
#endif

    // Cleanup Openssl
    opensslTest.openssl_cleanup();
}

/**
 * Setup the Secure server connection
 */
// void TcpServer::SetupSecureServerConnection()
// {

// }

/**
 * Connect to the TCP Server with SSL.
 *
 * TODO Fix this to not segfault in the while loop.
 * TODO Fix this to work again, moving things around broke this.
 *
 * Secure server testing.
 */
void TcpServer::RunServerSecure()
{
#ifdef SECURE_SERVER_TEST
    OpensslTest opensslTest;
    // This seems to work, TODO Do some more testing with this.
    // TODO Make this get the public and private key from a config file with Ini or Toml.

    // Step 1: Initialize OpenSSL
    opensslTest.init_openssl();

    if (server_fd == 0)
    {
        return;
    }

    // Step 2: Create TCP socket
    // int server_fd = opensslTest.create_tcp_socket(LISTEN_PORT);

    // Step 3-4: Create and configure SSL context
    // SSL_CTX *ctx = opensslTest.create_ssl_context();

    const char *publicKeyFile = PUBLIC_KEY;
    const char *privateKeyFile = PRIVATE_KEY;
    opensslTest.configure_ssl_context(ctx, publicKeyFile, privateKeyFile);

    // Accept TCP connection
    SetupClientConnection();

    // Create SSL object for this connection
    // SSL *ssl = SSL_new(ctx);
    // SSL_set_fd(ssl, client_fd); // Associate SSL object with client socket

    // Perform SSL handshake (negotiate secure session)
    if (SSL_accept(ssl) <= 0)
    { // Returns 1 on success
        log_output("Error: There was an error with the SSL connection.");
        ERR_print_errors_fp(stderr);
    }

    else
    {
        log_output("SSL handshake successful!");
        // printf("SSL handshake successful!\n");
        log_output("Using cipher: ", SSL_get_cipher(ssl));
        // printf("Using cipher: %s\n", SSL_get_cipher(ssl));  // Print negotiated cipher

        // Read data from client securely
        ReadFromClient(client_fd);
        // char buffer[BUFFER_SIZE];
        // int bytes_read = SSL_read(ssl, buffer, BUFFER_SIZE - 1);

        // if (bytes_read > 0)
        // {
        //     // Display the message from the client.
        //     buffer[bytes_read] = '\0';
        //     log_output("Received: ", buffer);
        //     // printf("Received: %s\n", buffer);

        //     // Send response securely
        //     const char *response = "Hello from secure server!";
        //     SSL_write(ssl, response, strlen(response));
    }

    // Cleanup
    // SSL_shutdown(ssl); // Gracefully close SSL session
    // SSL_free(ssl);     // Free SSL object
    // close(client_fd);  // Close TCP socket
    // SSL_CTX_free(ctx); // Free SSL context
    // close(server_fd);  // Close listening socket
    // EVP_cleanup();     // Cleanup OpenSSL algorithms

#endif // SECURE_SERVER_TEST
}

/**
 * Get the server fd value.
 */
const int TcpServer::GetServerFd() const
{
    return server_fd;
}

/**
 * Get the client fd value.
 */
const int TcpServer::GetClientFd() const
{
    return client_fd;
}

/**
 * Setup the connection to the client
 *
 * TODO Fix this to work here.
 * It gives a segfault if I try to use this for some reason.
 * Well it works as a void, but not an int.
 * 
 * TODO Fix this to only allow connections from specific IPs and methods, I can just send random
 *  requests with Postman to this.
 */
// int TcpServer::SetupClientConnection()
void TcpServer::SetupClientConnection()
{
    // Connection from the client.
    sockaddr_in client_address{};
    socklen_t client_len = sizeof(client_address);
    // int client_fd = accept(server_fd, reinterpret_cast<sockaddr *>(&client_address), &client_len);
    client_fd = accept(server_fd, reinterpret_cast<sockaddr *>(&client_address), &client_len);

    if (client_fd < 0)
    {
        perror("accept");
        close(server_fd);
        return;
        // return EXIT_FAILURE;
    }

    // auto ipAndPort = std::string(inet_ntoa(client_address.sin_addr)) + ":" + std::to_string(ntohs(client_address.sin_port));
    // log_output("Accepted TCP connection from {}: {}", ip, ntohs(client_addr.sin_port));
    // log_output("Accepted TCP connection from ", ipAndPort);
}

/**
 * Read the message from the client.
 */
void TcpServer::ReadFromClient(int client_fd)
{
    // Get the message from the client
    char buffer[1024] = {0};

#ifdef SECURE_SERVER_TEST
    int bytes_read = SSL_read(ssl, buffer, sizeof(buffer) - 1);
#else
    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
#endif

    if (bytes_read > 0)
    {
#ifdef SECURE_SERVER_TEST
        // Display the message from the client.
        buffer[bytes_read] = '\0';
        log_output("Received: ", buffer);
        SSL_write(ssl, buffer, strlen(buffer));
#else
        log_output("Received: ", buffer);
        Logger::getInstance().Log(LogLevel::LOG_INFO, "Received: " + std::string(buffer));
        send(client_fd, buffer, static_cast<size_t>(bytes_read), 0);

#endif
        // counter++;
        // log_output("Counter value: ", counter);
    }
}

/**
 * Run the TCP Server.
 *
 * TODO Add error handling to this.
 * TODO Add an option to stop the server, instead of having it loop forever.
 */
// void ConnectToServer(int argc, char *argv[])
int TcpServer::RunServer()
{
    OpensslTest opensslTest;
    // CustomAwaitable customAwaitable;

    // int counter = 1;
    // log_output("Counter value: ", counter);

    //-----------
    // Original methods, working.
    //-----------
    // Init Openssl
    // init_openssl();
    opensslTest.init_openssl();

    if (server_fd == 0)
    {
        return EXIT_FAILURE;
    }

    // Moved into constructor, revert if this fails.
    // server_fd = opensslTest.create_tcp_socket(LISTEN_PORT);

    // TODO Fix this to work.
    // client_fd = SetupClientConnection();



    // Well now, changing this from int to void fixed it? I'm not sure why...
    // Oh, maybe this was trying to connect to the client_fd set above..
    SetupClientConnection();

    // Coroutine test
    // customAwaitable.TestCoroutine();

    ReadFromClient(client_fd);

    // Close the socket
    // No wonder this is exiting right after a connection, it closes the connection right here.
    // TODO Make this able to receive multiple connections.

    return EXIT_SUCCESS;
}

#endif // __linux__
