#pragma once

#include <iostream>
#include <format>
#include <string>

// Enable extra debug logging for the server
#define EXTRA_LOGS

// Listen port for server
#define LISTEN_PORT 8055

// Size for buffer
#define BUFFER_SIZE 1024

// Program info
#define PROGRAM_NAME "KCNet-TcpServer"
#define PROGRAM_VERSION "v0.0.1a"

// Enable my SSL testing, required to be enabled on the Server and Client or it won't work.
// #define SECURE_SERVER_TEST

// Add the public and private key files here, these are required only for the server.
// I guess the client doesn't need these.
#define PUBLIC_KEY "server.crt"
#define PRIVATE_KEY "server.pem"


// This works for logging with a message
template <typename T, typename... Ts>
inline void log_output(const T& first, const Ts&... rest)
{
    // std::cout << "[TcpServer]: " << first;
    std::cout << "[" << PROGRAM_NAME << "]: " << first;
    ((std::cout << rest), ...);   // print each remaining argument
    std::cout << std::endl;
}

// template <typename... Ts>
// inline void log_output(std::string_view fmt, Ts&&... args)
// {
//     std::cout << "[TcpServer]: " << std::vformat(fmt, std::make_format_args(args...)) << "\n";
// }
