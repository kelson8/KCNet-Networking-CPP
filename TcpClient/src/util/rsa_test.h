#pragma once

#include "defines.h"

#ifdef __linux__

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <openssl/evp.h>
#include <openssl/pem.h>

#endif // __linux__

class RsaTest {

public:
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

    unsigned char* GetPublicKey(EVP_PKEY *key);
    unsigned char* GetPrivateKey(EVP_PKEY *key);
    void ReadKeyFromFile();
    void TestRsa();

    void NewRsaTest();

private:
#ifdef __linux__
    EVP_PKEY &CreatePrivateKey();
#endif // __linux__
};