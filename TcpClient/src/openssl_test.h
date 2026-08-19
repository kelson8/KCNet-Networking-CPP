#pragma once

#ifdef __linux__

#include "defines.h"

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <openssl/evp.h>
#include <openssl/pem.h>

class OpensslTest
{
public:
    static void init_openssl(void);
    int create_tcp_client(const char *server_ip, int port);
    SSL_CTX *create_client_ssl_context();

    bool Encrypt(const unsigned char *plaintext,
                 int plaintext_len,
                 const unsigned char *key,
                 const unsigned char *iv,
                 unsigned char *ciphertext,
                 int &ciphertext_len);

    bool Decrypt(const unsigned char *ciphertext,
                 int ciphertext_len,
                 const unsigned char *key,
                 const unsigned char *iv,
                 unsigned char *plaintext,
                 int &plaintext_len);

    void PublicKeyTesting();
    std::string ReadPrivateKey(const char* privateKeyFile);
    void TestRsa();

    void NewRsaTest();

private:
    EVP_PKEY &CreatePrivateKey();
};

#endif // __linux__