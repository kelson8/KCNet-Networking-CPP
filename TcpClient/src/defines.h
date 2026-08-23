#pragma once

#include <iostream>
#include <format>
#include <string>

// Enable my SSL testing, required to be enabled on the Server and Client or it won't work.
// #define SECURE_SERVER_TEST

// Switch to using the wxwidgets gui for the client
// This needs some work done before it's ready to be used.
// TODO Figure out how to disable this when building with Docker.
#define WXWIDGETS_GUI

// Values for wxwidgets
// Set the Window width and height here, such as 1280x720.
#define WINDOW_WIDTH 450
#define WINDOW_HEIGHT 300

// Enable the url checker testing to validate urls.
// Not implemented yet.
// #define URL_CHECKER_TEST

// Enable the Curl testing
// I will be using this eventually to check if the server is online before trying to connect to it.
// This should only be set in TcpClient CMakeLists with the ENABLE_CURL option.
// Keeping this here so I remember what it does.
// #define CURL_TEST

// Set the server connection info.
#define SERVER_PORT 8055
#define SERVER_IP "192.168.1.108"

// Size for buffer
#define BUFFER_SIZE 1024

// Program info
#define PROGRAM_NAME "KCNet-TcpClient"
#define PROGRAM_VERSION "v0.0.1a"
#define LOG_PREFIX "[KCNet-TcpClient]:"

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
