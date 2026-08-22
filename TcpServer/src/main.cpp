
#include <string>
#include "defines.h"

#include <fmt/format.h>
#include <random>

#include "util.h"

#ifdef __linux__
// TODO Look into this later.
// https://github.com/embeddedmz/socket-cpp

#include "openssl_test.h"
#include "file_functions.h"
#include "toml_handler.h"

#include "tcp_server.h"

// TODO Add this to Git
// #include "command_system.h"

#include <thread>
#include <bits/stdc++.h>

#endif // __linux__

//--------------
// Server for TCP testing.
//--------------

// TODO Look into this
// I might be able to allow multiple clients to connect to this.
// https://www.geeksforgeeks.org/cpp/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/

// https://www.geeksforgeeks.org/cpp/socket-programming-in-cpp/

// Here is the guide I used for this basic networking test, which seems to work well.
// https://codezup.com/c-networking-guide/

// TODO Look into this guide, they are using TLS
// I implemented some of this.
// https://www.techbuddies.io/2026/01/02/how-to-build-a-c-openssl-tls-client-server-over-tcp-and-udp/

// Other useful guides
// https://www.techbuddies.io/2025/10/03/lets-build-a-reliable-server-app-with-c-sockets/
//

// TODO Look into this TLS guide
// https://linuxvox.com/blog/turn-a-simple-socket-into-an-ssl-socket/

// TODO Look into this TLS Server library which has a client and a server side part like I am doing.
// This also works in Windows too and the project is MIT licensed.
// https://github.com/embeddedmz/socket-cpp

// New for TLS

// To genereate TLS keys for this:
/*
# 1. Generate a 2048-bit RSA private key
openssl genrsa -out server.key 2048
# 2. Create a self-signed certificate valid for 365 days
openssl req -new -x509 -key server.key -out server.crt \
    -days 365 -subj "/C=US/ST=Test/L=Local/O=DevOrg/OU=Dev/CN=localhost"

# 3. Copy files into build folder
cp keyname.* ./build/TcpServer

TODO Automate copying keys into build folder later for testing.
*/

// If the server should run
bool runProgram = true;

#ifdef __linux__

// https://stackoverflow.com/questions/4250013/is-destructor-called-if-sigint-or-sigstp-issued
std::atomic<bool> quit(false); // signal flag

/**
 * For destructor calling with SIGINT on Linux
 * This works for calling destructors that would normally not be called.
 */
void got_signal(int)
{
    // Signal handler function.
    // Set the flag and return.
    // Never do real work inside this function.
    // See also: man 7 signal-safety
    quit.store(true);
}

#endif // __linux__

int main(int argc, char *argv[])
{
    // https://www.w3schools.com/cpp/cpp_howto_random_number.asp
    // Seed the random number generator.
    srand(time(0));

    Util &util = Util::getInstance();
    util.SetServerRunning(true);

#ifdef __linux__

    // log_output("Program PID is: ", get_program_pid());

    // For destructor calling with SIGINT on Linux
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = got_signal;
    sigfillset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    //

    // TODO Add this to Git
    // Command argument system, this will eventually be able to set the IP, port, and config file location.
    // CommandSystem::ArghCommandSystem(argc, argv);
    //

    // TODO Implement multithreading for this.
    // https://www.geeksforgeeks.org/cpp/multithreading-in-cpp/
    // std::thread thread2(print_keycode);

    // TcpServer &tcpServer = TcpServer::getInstance();

    // Random number generator test.
    // fmt::print("Random number: {}\n", util.GenerateRandomNumber(10, 1000));

    // Run the toml file testing.
    // TomlHandler &tomlHandler = TomlHandler::getInstance();

    // tomlHandler.TestTomlFile();

    // If this gets the shutdown command from the client, it will shut the server down.
    while (util.GetServerRunning())
    {

#ifdef SECURE_SERVER_TEST

        // Run the secure server
        // TODO Fix this in the while loop, if I try to send multiple connections this crashes.
        // TODO Fix the secure server loop.
        tcpServer.RunServerSecure();
        // }
#else

        // Now with the constructor and destructor setup, this can now run in a while loop!
        // if (tcpServer.RunServer() != EXIT_SUCCESS)
        if (TcpServer::getInstance().RunServer() != EXIT_SUCCESS)
        {
            return EXIT_FAILURE;
        }

        // Main thread waits for 'thread2' to finish
        // TODO Fix this thread, make this close by pressing 'Q' and cleanup.
        // thread2.join();
        // }

        // Get the values from the TcpServer file.
        // This runs after the RunServer gets a message.
        // log_output("ServerFD value: ", tcpServer.GetServerFd());
        // log_output("ClientFD value: ", tcpServer.GetClientFd());

        // while (true)
        // {
        //     if (tcpServer.RunServer()  != EXIT_SUCCESS)

        // }

#endif // SECURE_SERVER_TEST

        // Handler for SIGINT and other signals.
        if (quit.load())
            break;
    }

#else
    std::cerr << "Error: This is only supported on Linux!" << std::endl;
#endif // __linux__

    return EXIT_SUCCESS;
}
