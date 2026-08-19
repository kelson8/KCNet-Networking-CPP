
#ifdef __linux__
#include <string>

#include "urandom_test.h"

#include "defines.h"
#include "tcp_client.h"

#include "base64.hpp"
#include "openssl_test.h"

#endif // __linux__

//--------------
// Client for TCP testing.
//--------------

// https://www.geeksforgeeks.org/cpp/socket-programming-in-cpp/

// Here is the guide I used for this basic networking test, which seems to work well.
// https://codezup.com/c-networking-guide/

// If this is set to false, it disables the connection to the server.
bool connectionEnabled = true;


int main(int argc, char *argv[])
{

#ifdef __linux__
    OpensslTest opensslTest;
    TcpClient &tcpClient = TcpClient::getInstance();

    // opensslTest.PublicKeyTesting();
    // opensslTest.TestRsa();

    // opensslTest.ReadPrivateKey("server.pem");

#ifdef SECURE_SERVER_TEST

    tcpClient.ConnectToServerSecure();

#else

    //-----------
    // Original methods, working.
    //-----------
    // If the connection isnt enabled, run the below argument testing.
    if (!connectionEnabled)
    {
        // https://www.geeksforgeeks.org/cpp/command-line-arguments-in-cpp/
        std::cout << "You have entered " << argc << " arguments:" << std::endl;

        // Using a while loop to
        // iterate through arguments
        int i = 0;
        while (i < argc)
        {
            std::cout << "Argument " << i + 1 << ": " << argv[i] << std::endl;
            i++;
        }

        // std::cout << "Tcp Client connection is disabled" << std::endl;
        return 0;
    }

    // tcpClient.ConnectToServer(argc, argv);

    if(tcpClient.ConnectToServer() != EXIT_SUCCESS)
    {
        return EXIT_FAILURE;
    }

#endif // SECURE_SERVER_TEST

#else
    std::cerr
        << "Error: This is only supported on Linux!" << std::endl;
#endif // __linux__

    return 0;
}
