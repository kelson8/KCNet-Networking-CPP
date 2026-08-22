#pragma once

#include <iostream>
#include <format>
#include <string>

// Enable my SSL testing, required to be enabled on the Server and Client or it won't work.
// #define SECURE_SERVER_TEST

// Switch to using the wxwidgets gui for the client
// This needs some work done before it's ready to be used.
// #define WXWIDGETS_GUI

// Set the server connection info.
#define SERVER_PORT 8055
#define SERVER_IP "192.168.1.108"

// Size for buffer
#define BUFFER_SIZE 1024

// Program info
#define PROGRAM_NAME "KCNet-TcpClient"
#define PROGRAM_VERSION "v0.0.1a"

// Enable the new server test with the library.
// https://github.com/embeddedmz/socket-cpp
// TODO Fix this to work.
// #define NEW_SERVER_TEST

// This works for logging with a message
template <typename T, typename... Ts>
inline void log_output(const T& first, const Ts&... rest)
{
    // std::cout << "[TcpClient]: " << first;
    std::cout << "[" << PROGRAM_NAME << "]: " << first;
    ((std::cout << rest), ...);   // print each remaining argument
    std::cout << std::endl;
}

// template <typename... Ts>
// inline void log_output(std::string_view fmt, Ts&&... args)
// {
//     std::cout << "[TcpClient]: " << std::vformat(fmt, std::make_format_args(args...)) << "\n";
// }
