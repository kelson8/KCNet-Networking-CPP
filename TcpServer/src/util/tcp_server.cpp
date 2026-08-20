

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
 * Non blocking accept, TODO Fix this.
 * https://oneuptime.com/blog/post/2026-03-20-non-blocking-tcp-socket-c/view
 */
void TcpServer::AcceptAll(int server_fd)
{
    while (1)
    {
        struct sockaddr_in client_address{};
        socklen_t client_len = sizeof(client_address);

        /* accept4() with SOCK_NONBLOCK sets the new socket non-blocking atomically */
        // client_fd = accept4(server_fd, (struct sockaddr *)&client_address, &client_len, SOCK_NONBLOCK);
        client_fd = accept4(server_fd, reinterpret_cast<sockaddr *>(&client_address), &client_len, SOCK_NONBLOCK);
        if (client_fd < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break; /* no more pending connections */
            }
            perror("accept4");
            break;
        }
        /* handle cfd (add to epoll/select, etc.) */
    }
}

/**
 * Non-blocking send() may write fewer bytes than requested (EAGAIN when buffer full).
 * Retry with the unsent remainder.
 * 
 * This works for non-blocking send.
*/
ssize_t TcpServer::SendNonBlocking(int fd, const char *buf, size_t len)
{
    size_t sent = 0;
    while (sent < len)
    {
        ssize_t s = send(fd, buf + sent, len - sent, 0);
        if (s > 0)
        {
            sent += (size_t)s;
        }
        else if (s < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                /* Socket send buffer full - register EPOLLOUT and retry later */
                return (ssize_t)sent;
            }
            perror("send");
            return -1;
        }
    }
    return (ssize_t)sent;
}

/**
 * Get the connecting client IP
 * 
 * https://stackoverflow.com/questions/3060950/how-to-get-ip-address-from-sock-structure-in-c
 */
const std::string TcpServer::GetClientIp() const
{
    // TODO Test this, not sure if this will work.
    if (client_address.sin_family == NULL)
        return "Invalid IP Address";

    socklen_t client_len = sizeof(client_address);
    char ip_address[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_address.sin_addr), ip_address, client_len);
    return ip_address;
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
    // sockaddr_in client_address{};
    socklen_t client_len = sizeof(client_address);
    client_fd = accept(server_fd, reinterpret_cast<sockaddr *>(&client_address), &client_len);

    // TODO Fix this to work.
    // AcceptAll(server_fd);

    if (client_fd < 0)
    {
        perror("accept");
        close(server_fd);
        return;
        // return EXIT_FAILURE;
    }

    // Log the client IP, this works.
    log_output("Client IP: ", GetClientIp());

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


        // TODO Fix this to actually do something, possibly reload a config file.
        // Dummy reload command
        if(std::string(buffer) == "RELOAD")
        {
            log_output("Reload command received from client, reloading server...");
        }
        else
        {
            // Any message not listed above should also be logged.
            log_output("Received: ", buffer);
            Logger::getInstance().Log(LogLevel::LOG_INFO, "Received: " + std::string(buffer));
        }

        // Switched to using non blocking send command.
        // send(client_fd, buffer, static_cast<size_t>(bytes_read), 0);
        SendNonBlocking(client_fd, buffer, static_cast<size_t>(bytes_read));

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

    // This is why this constantly gives the accept: Bad file descriptor errors I think.
    // It keeps trying to loop and run this function..
    // This only happens if I enable non blocking for the server though.
    SetupClientConnection();

    // Coroutine test
    // customAwaitable.TestCoroutine();

    ReadFromClient(client_fd);

    return EXIT_SUCCESS;
}

#endif // __linux__
