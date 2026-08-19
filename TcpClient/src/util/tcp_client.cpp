
#ifdef __linux__
#include "tcp_client.h"

#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <openssl/evp.h>
#include <openssl/pem.h>

#include <string>

#include "urandom_test.h"

#include "defines.h"

#ifdef NEW_SERVER_TEST
#include "TCPClient.h"
#include "TCPServer.h"
#include "TCPSSLServer.h"
#include "TCPSSLClient.h"
#endif // NEW_SERVER_TEST

#include "base64.hpp"
#include "openssl_test.h"

#include "file_functions.h"

/**
 * Structure for the future packets that I will be sending to the server.
 * I will be sending the public key, and message to encrypt with this.
 * 
 * TODO Fix this to be used.
 */
struct EncryptionPacket {
    // const char* publicKey;
    const unsigned char* key;
    // uint8_t key;
    // const char* messageToEncrypt;
    const char* message;
};

/**
 * For RSA message decryption.
 * I will be sending the private key, and message to decrypt with this.
 */
// struct DecryptionPacket {
//     const char* privateKey;
//     const char* messageToDecrypt;
// };

//--------------
// Client for TCP testing.
//--------------

TcpClient::TcpClient()
{

}

/**
 * Convert the key from base64 back to text.
 */
// void DecodeKeyBase64(EncryptionPacket packet)
void DecodeKeyBase64(const char* text)
{
    // auto decoded = base64::from_base64(reinterpret_cast<const char*>(packet.key));
    auto decoded = base64::from_base64(text);
    log_output("Decoded text: ", decoded);
}

/**
 * Make the key into base64.
 */
// void EncodeKeyBase64(EncryptionPacket packet)
void EncodeKeyBase64(const unsigned char* text)
{
    // auto encoded = base64::to_base64(packet.key, sizeof(packet.key));
    auto encoded = base64::to_base64(text, sizeof(text));
    log_output("Encoded text: ", encoded);
}

/**
 * Connect to the TCP Server with SSL.
 * 
 * Secure server testing.
 */
void TcpClient::ConnectToServerSecure()
{
#ifdef SECURE_SERVER_TEST
    OpensslTest opensslTest;
        // New testing
    // Well this no longer works with my other testing.
    // Step 1: Connect to server via TCP
    int sockfd = opensslTest.create_tcp_client(SERVER_IP, SERVER_PORT);
 
    // Step 2: Initialize OpenSSL
    // init_openssl();
    opensslTest.init_openssl();
 
    // Step 3: Create SSL context and object
    // SSL_CTX* ctx = create_client_ssl_context();
    SSL_CTX* ctx = opensslTest.create_client_ssl_context();
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);  // Associate socket with SSL
 
    // Step 4: Perform SSL handshake
    if (SSL_connect(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
    } else {
        log_output("SSL handshake successful!");
        log_output("Using cipher: ", SSL_get_cipher(ssl));

        // printf("SSL handshake successful!\n");
        // printf("Using cipher: %s\n", SSL_get_cipher(ssl));
 
        // Send data securely
        const char* message = "Hello from secure client!";
        SSL_write(ssl, message, strlen(message));
 
        // Read response
        char buffer[BUFFER_SIZE];
        int bytes_read = SSL_read(ssl, buffer, BUFFER_SIZE - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            log_output("Server response: ", buffer);
            // printf("Server response: %s\n", buffer);
        }
    }
 
    // Cleanup
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sockfd);
    SSL_CTX_free(ctx);
    EVP_cleanup();
    //

#endif // SECURE_SERVER_TEST
}

// void SendMessageToServer(int socket, const char* message, size_t messageLength, int flags)

/**
 * Send a message to the TCP server
 * socket The network socket to send the message to.
 * message The message to send.
 * messageLength The length of the message to send.
 */
void TcpClient::SendMessageToServer(int socket, const char* message, size_t messageLength)
{
    // Last parameter is flags.
    send(socket, message, messageLength, 0);
}

/**
 * Connect to the TCP Server.
 * 
 * TODO Fix this to give a proper error if the server is offline.
 */
// void ConnectToServer(int argc, char *argv[])
int TcpClient::ConnectToServer()
{
    Urandom urandom;

    OpensslTest opensslTest;

    FileFunctions &fileFunctions = FileFunctions::getInstance();

    // TODO Fix this, it gives invalid conversion from const char* to const unsigned char*
    // EncryptionPacket packet;
    // // const char* packetText = "Test Key, will need to be read from file";
    // char packetText[] = "Test Key, will need to be read from file";
    // unsigned char packetTextBuffer[512];

    // packet.message = "Test message";

    // // https://www.geeksforgeeks.org/c/strncpy-function-in-c/
    // strcpy(packetText, reinterpret_cast<const char*>(packetTextBuffer));

    // packet.key = packetTextBuffer;

    // packet.key = "Test Key, will need to be read from file";
    // EncodeKeyBase64("Test");
    // DecodeKeyBase64("xIDAEMl3AAA=");


    // The input for the user, works in a bash prompt.
    // If the value is in all quotes it seems to work.
    // Text such as "Welcome to the TcpTest." will display with this.
    // Well if I have no parameters then this crashes it due to it being null.
    // std::string userInput = argv[1];

    // Generate random characters for testing sending a message with
    int amountOfNumbers = 4;
    int amountOfLetters = 10;
    int amountOfSymbols = 4;

    std::string randomValue = urandom.GenerateRandom(amountOfNumbers, amountOfLetters, amountOfSymbols);
    // urandom.Run();

    //

    // Connect to the TCP server.
    int sock = opensslTest.create_tcp_client(SERVER_IP, SERVER_PORT);

    // const char *message = "Hello from client";

    // Now this sends a value from /dev/urandom to the server.
    const char *message = randomValue.c_str();
    
    // Struct testing
    // RsaMessageEncrypt rsaEncrypt;
    // rsaEncrypt.messageToEncrypt = "dd";
    // rsaEncrypt.publicKey = "dd";

    // Send a message from the text file.
    // TODO Fix this to work properly.
    // bool doesFileExist = fileFunctions.FileExists("test.txt");
    // if(!doesFileExist)
    // {
        // log_output("Error: The file doesn't exist.");
        // return EXIT_FAILURE;
    // }

    // const char* message = fileFunctions.ReadFile("test.txt").c_str();
    // log_output(message);

    // log_output(fileFunctions.ReadFile("test.txt"));

    // Well I can easily send custom messages with this.
    // TODO Fix error handling for this.
    // const char *message = argv[1];
    // if(message == NULL)
    // {
    //     log_output("Error: Message was blank.");
    //     return;
    // }

    SendMessageToServer(sock, message, strlen(message));
    
    // Read the bytes back from the server, or say message was sent.
    char buffer[1024] = {0};
    ssize_t bytes_read = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read > 0)
    {
        // std::cout << "Server replied: " << buffer << "\n";
        // log_output("Sent message to server: ", buffer);
        log_output("Sent message to server.");
        return EXIT_SUCCESS;
    }

    close(sock);
    return EXIT_FAILURE;
}

#endif // __linux__
