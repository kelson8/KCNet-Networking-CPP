#include "rsa_test.h"

#ifdef __linux__

#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>

#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>

#include <fstream>
#include <sstream>

#include <memory>
#include <string>

// #include "decrypt.h"
// #include "encrypt.h"
// #include "keyPair.h"

#include "base64.hpp"
#include "file_functions.h"

//---------
// RSA
//---------

// Smart pointers to wrap openssl C types that need explicit free
using BIO_ptr = std::unique_ptr<BIO, decltype(&BIO_free)>;
using X509_ptr = std::unique_ptr<X509, decltype(&X509_free)>;
using ASN1_TIME_ptr = std::unique_ptr<ASN1_TIME, decltype(&ASN1_STRING_free)>;

/**
 * Create a private key
 * TODO Test this
 */
EVP_PKEY &RsaTest::CreatePrivateKey()
{
    // create private/public key pair
    // init RSA context, so we can generate a key pair
    EVP_PKEY_CTX *keyCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    EVP_PKEY_keygen_init(keyCtx);
    EVP_PKEY_CTX_set_rsa_keygen_bits(keyCtx, 4096); // RSA 4096
    // variable that will hold both private and public keys
    EVP_PKEY *key = NULL;
    // generate key
    EVP_PKEY_keygen(keyCtx, &key);
    return *key;
    // free up key context
    EVP_PKEY_CTX_free(keyCtx);
}

// https://friendlyuser.github.io/posts/tech/cpp/Using_OpenSSL_in_C++_A_Comprehensive_Guide/
// TODO Figure out how to use these encrypt and decrypt functions with X509.

bool RsaTest::Encrypt(const unsigned char *plaintext,
                      int plaintext_len,
                      const unsigned char *key,
                      const unsigned char *iv,
                      unsigned char *ciphertext,
                      int &ciphertext_len)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        std::cerr << "Failed to create EVP_CIPHER_CTX" << std::endl;
        return false;
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, key, iv) != 1)
    {
        std::cerr << "Failed to initialize encryption" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    if (EVP_EncryptUpdate(ctx, ciphertext, &ciphertext_len, plaintext, plaintext_len) != 1)
    {
        std::cerr << "Failed to encrypt data" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

bool RsaTest::Decrypt(const unsigned char *ciphertext,
                      int ciphertext_len,
                      const unsigned char *key,
                      const unsigned char *iv,
                      unsigned char *plaintext,
                      int &plaintext_len)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        std::cerr << "Failed to create EVP_CIPHER_CTX" << std::endl;
        return false;
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, key, iv) != 1)
    {
        std::cerr << "Failed to initialize decryption" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    if (EVP_DecryptUpdate(ctx, plaintext, &plaintext_len, ciphertext, ciphertext_len) != 1)
    {
        std::cerr << "Failed to decrypt data" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    EVP_CIPHER_CTX_free(ctx);
    return true;
}
//

/***
 *
 * New testing with RSA.
 *
 * https://hayageek.com/rsa-encryption-decryption-openssl-c/
 */

int padding = RSA_PKCS1_PADDING;

/**
 * Create the RSA object
 */
RSA *createRSA(unsigned char *key, bool isPublicKey)
{
    RSA *rsa = NULL;
    BIO *keybio;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio == NULL)
    {
        // printf("Failed to create key BIO");
        log_output("Error: Failed to create key BIO");
        return 0;
    }
    if (isPublicKey)
    {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
    }
    else
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
    }
    if (rsa == NULL)
    {
        log_output("Error: Failed to create RSA");
        // printf("Failed to create RSA");
    }
    return rsa;
}

/**
 * Encrypt with the public key.
 */
int public_encrypt(unsigned char *data, int data_len, unsigned char *key, unsigned char *encrypted)
{
    RSA *rsa = createRSA(key, true);
    int result = RSA_public_encrypt(data_len, data, encrypted, rsa, padding);
    return result;
}

/**
 * Decrypt with the private key.
 */
int private_decrypt(unsigned char *enc_data, int data_len, unsigned char *key, unsigned char *decrypted)
{
    RSA *rsa = createRSA(key, false);
    int result = RSA_private_decrypt(data_len, enc_data, decrypted, rsa, padding);
    return result;
}

/**
 * Encrypt with the private key.
 */
int private_encrypt(unsigned char *data, int data_len, unsigned char *key, unsigned char *encrypted)
{
    RSA *rsa = createRSA(key, false);
    int result = RSA_private_encrypt(data_len, data, encrypted, rsa, padding);
    return result;
}

/**
 * Decrypt with the public key.
 */
int public_decrypt(unsigned char *enc_data, int data_len, unsigned char *key, unsigned char *decrypted)
{
    RSA *rsa = createRSA(key, true);
    int result = RSA_public_decrypt(data_len, enc_data, decrypted, rsa, padding);
    return result;
}

// Example new RSA usage

/**
 * This works, although I did a lot of this
 * reinterpret_cast<unsigned char*>
 *
 * Not sure if this casting will effect anything.
 */
// void TestRsa()
void RsaTest::TestRsa()
{
    char plainText[2048 / 8] = "Welcome to RSA HELL"; // key length : 2048
    char publicKey[] = "-----BEGIN PUBLIC KEY-----\n"
                       "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAy8Dbv8prpJ/0kKhlGeJY\n"
                       "ozo2t60EG8L0561g13R29LvMR5hyvGZlGJpmn65+A4xHXInJYiPuKzrKUnApeLZ+\n"
                       "vw1HocOAZtWK0z3r26uA8kQYOKX9Qt/DbCdvsF9wF8gRK0ptx9M6R13NvBxvVQAp\n"
                       "fc9jB9nTzphOgM4JiEYvlV8FLhg9yZovMYd6Wwf3aoXK891VQxTr/kQYoq1Yp+68\n"
                       "i6T4nNq7NWC+UNVjQHxNQMQMzU6lWCX8zyg3yH88OAQkUXIXKfQ+NkvYQ1cxaMoV\n"
                       "PpY72+eVthKzpMeyHkBn7ciumk5qgLTEJAfWZpe4f4eFZj/Rc8Y8Jj2IS5kVPjUy\n"
                       "wQIDAQAB\n"
                       "-----END PUBLIC KEY-----\n";

    // TODO Figure out how to switch to this for reading a private and public key here.
    // const char *privateKey1 = ReadPrivateKey("server.pem").copy();
    // char privateKey1 = ReadPrivateKey("server.pem");

    // I think this works!
    //  Mostly from this guide below
    //  https://stackoverflow.com/questions/2206050/how-to-convert-a-string-literal-to-unsigned-char-array-in-visual-c
    //  Other guide for this:
    //  https://www.techiedelight.com/convert-std-string-char-cpp/

    // Reading a private key into an unsigned char
    unsigned char privateKeyTest[2048];
    // char privateKeyTest[2048];
    // strcpy((char*) privateKeyTest, ReadPrivateKey("server.pem").c_str());
    // Instead of '(char*)', I can use 'static_cast <char*>'
    // Well this just gives a segfault when trying to be used instead of the below privateKey value.
    // strcpy((char *)privateKeyTest, ReadPrivateKey("server.pem").c_str());
    // strcpy((char *)privateKeyTest, GetPrivateKey("server.pem").c_str());

    // Test for private key reading.
    // This displays the private key.
    // I could use this same function for reading the public key.
    // std::cout << privateKeyTest << std::endl;
    //

    char privateKey[] = "-----BEGIN RSA PRIVATE KEY-----\n"
                        "MIIEowIBAAKCAQEAy8Dbv8prpJ/0kKhlGeJYozo2t60EG8L0561g13R29LvMR5hy\n"
                        "vGZlGJpmn65+A4xHXInJYiPuKzrKUnApeLZ+vw1HocOAZtWK0z3r26uA8kQYOKX9\n"
                        "Qt/DbCdvsF9wF8gRK0ptx9M6R13NvBxvVQApfc9jB9nTzphOgM4JiEYvlV8FLhg9\n"
                        "yZovMYd6Wwf3aoXK891VQxTr/kQYoq1Yp+68i6T4nNq7NWC+UNVjQHxNQMQMzU6l\n"
                        "WCX8zyg3yH88OAQkUXIXKfQ+NkvYQ1cxaMoVPpY72+eVthKzpMeyHkBn7ciumk5q\n"
                        "gLTEJAfWZpe4f4eFZj/Rc8Y8Jj2IS5kVPjUywQIDAQABAoIBADhg1u1Mv1hAAlX8\n"
                        "omz1Gn2f4AAW2aos2cM5UDCNw1SYmj+9SRIkaxjRsE/C4o9sw1oxrg1/z6kajV0e\n"
                        "N/t008FdlVKHXAIYWF93JMoVvIpMmT8jft6AN/y3NMpivgt2inmmEJZYNioFJKZG\n"
                        "X+/vKYvsVISZm2fw8NfnKvAQK55yu+GRWBZGOeS9K+LbYvOwcrjKhHz66m4bedKd\n"
                        "gVAix6NE5iwmjNXktSQlJMCjbtdNXg/xo1/G4kG2p/MO1HLcKfe1N5FgBiXj3Qjl\n"
                        "vgvjJZkh1as2KTgaPOBqZaP03738VnYg23ISyvfT/teArVGtxrmFP7939EvJFKpF\n"
                        "1wTxuDkCgYEA7t0DR37zt+dEJy+5vm7zSmN97VenwQJFWMiulkHGa0yU3lLasxxu\n"
                        "m0oUtndIjenIvSx6t3Y+agK2F3EPbb0AZ5wZ1p1IXs4vktgeQwSSBdqcM8LZFDvZ\n"
                        "uPboQnJoRdIkd62XnP5ekIEIBAfOp8v2wFpSfE7nNH2u4CpAXNSF9HsCgYEA2l8D\n"
                        "JrDE5m9Kkn+J4l+AdGfeBL1igPF3DnuPoV67BpgiaAgI4h25UJzXiDKKoa706S0D\n"
                        "4XB74zOLX11MaGPMIdhlG+SgeQfNoC5lE4ZWXNyESJH1SVgRGT9nBC2vtL6bxCVV\n"
                        "WBkTeC5D6c/QXcai6yw6OYyNNdp0uznKURe1xvMCgYBVYYcEjWqMuAvyferFGV+5\n"
                        "nWqr5gM+yJMFM2bEqupD/HHSLoeiMm2O8KIKvwSeRYzNohKTdZ7FwgZYxr8fGMoG\n"
                        "PxQ1VK9DxCvZL4tRpVaU5Rmknud9hg9DQG6xIbgIDR+f79sb8QjYWmcFGc1SyWOA\n"
                        "SkjlykZ2yt4xnqi3BfiD9QKBgGqLgRYXmXp1QoVIBRaWUi55nzHg1XbkWZqPXvz1\n"
                        "I3uMLv1jLjJlHk3euKqTPmC05HoApKwSHeA0/gOBmg404xyAYJTDcCidTg6hlF96\n"
                        "ZBja3xApZuxqM62F6dV4FQqzFX0WWhWp5n301N33r0qR6FumMKJzmVJ1TA8tmzEF\n"
                        "yINRAoGBAJqioYs8rK6eXzA8ywYLjqTLu/yQSLBn/4ta36K8DyCoLNlNxSuox+A5\n"
                        "w6z2vEfRVQDq4Hm4vBzjdi3QfYLNkTiTqLcvgWZ+eX44ogXtdTDO7c+GeMKWz4XX\n"
                        "uJSUVL5+CVjKLjZEJ6Qc2WZLl94xSwL71E41H4YciVnSCQxVc4Jw\n"
                        "-----END RSA PRIVATE KEY-----\n";

    // Store the messages
    unsigned char encrypted[4098] = {};
    unsigned char decrypted[4098] = {};

    int encrypted_length = public_encrypt(reinterpret_cast<unsigned char *>(plainText), strlen(plainText), reinterpret_cast<unsigned char *>(publicKey), encrypted);

    if (encrypted_length == -1)
    {
        // printLastError("Public Encrypt failed ");
        log_output("Public Encrypt failed");
        exit(0);
    }

    int decrypted_length = private_decrypt(encrypted, encrypted_length, reinterpret_cast<unsigned char *>(privateKey), decrypted);
    // int decrypted_length = private_decrypt(encrypted, encrypted_length, privateKey, decrypted);

    if (decrypted_length == -1)
    {

        log_output("Public Decrypt failed");
        exit(0);
    }

    // Private decrypt
    encrypted_length = private_encrypt(reinterpret_cast<unsigned char *>(plainText), strlen(plainText), reinterpret_cast<unsigned char *>(privateKey), encrypted);
    // encrypted_length = private_encrypt(reinterpret_cast<unsigned char *>(plainText), strlen(plainText), privateKey, encrypted);

    if (encrypted_length == -1)
    {
        log_output("Private Encrypt failed");
        exit(0);
    }

    // printf("Encrypted length = %d\n", encrypted_length);
    log_output("Encrypted length: ", encrypted_length);

    // Public decrypt
    decrypted_length = public_decrypt(encrypted, encrypted_length, reinterpret_cast<unsigned char *>(publicKey), decrypted);

    if (decrypted_length == -1)
    {
        log_output("Public Decrypt failed");
        exit(0);
    }

    // printf("Decrypted Text = %s\n", decrypted);
    // printf("Decrypted Length = %d\n", decrypted_length);

    log_output("Decrypted Text: ", decrypted);
    log_output("Decrypted Length: ", decrypted_length);
}

//-------

/**
 * Get the public key as a string
 * 
 * @param key The EVP_PKEY pointer.
 */
unsigned char* RsaTest::GetPublicKey(EVP_PKEY *key)
{
    // create a place to dump the IO, in this case in memory
    BIO *publicBIO = BIO_new(BIO_s_mem());

    // dump key to IO
    PEM_write_bio_PUBKEY(publicBIO, key);

    // get buffer length
    int publicKeyLen = BIO_pending(publicBIO);

    // create char reference of public key length
    unsigned char *publicKeyChar = (unsigned char *)malloc(publicKeyLen);

    // read the key from the buffer and put it in the char reference
    BIO_read(publicBIO, publicKeyChar, publicKeyLen);
    // at this point we can save the public somewhere

    // pretend we are pulling the public key from some source and using it
    // to encrypt a message
    unsigned char *rsaPublicKeyChar = publicKeyChar;

    // write char array to BIO
    BIO *rsaPublicBIO = BIO_new_mem_buf(rsaPublicKeyChar, -1);

    // create a RSA object from public key char array
    RSA *rsaPublicKey = NULL;
    PEM_read_bio_RSA_PUBKEY(rsaPublicBIO, &rsaPublicKey, NULL, NULL);

    // create public key
    // TODO Make a function for this.
    // EVP_PKEY *publicKey = EVP_PKEY_new();
    // EVP_PKEY_assign_RSA(publicKey, rsaPublicKey);

    return publicKeyChar;


}

/**
 * Get the private key as a string
 * 
 * @param key The EVP_PKEY pointer.
 */
unsigned char* RsaTest::GetPrivateKey(EVP_PKEY *key)
{
    // EVP_PKEY *key = NULL;

    // create a RSA object from private key char array
    RSA *rsaPrivateKey = NULL;

    // create a place to dump the IO, in this case in memory
    BIO *privateBIO = BIO_new(BIO_s_mem());
    // dump key to IO
    PEM_write_bio_PrivateKey(privateBIO, key, NULL, NULL, 0, 0, NULL);
    // get buffer length
    int privateKeyLen = BIO_pending(privateBIO);
    // create char reference of private key length
    unsigned char *privateKeyChar = (unsigned char *)malloc(privateKeyLen);
    // read the key from the buffer and put it in the char reference
    BIO_read(privateBIO, privateKeyChar, privateKeyLen);

    // at this point we can save the private key somewhere
    return privateKeyChar;
}

void RsaTest::ReadKeyFromFile()
{
    // init RSA context, so we can generate a key pair
    // EVP_PKEY_CTX *keyCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    // EVP_PKEY_keygen_init(keyCtx);
    // EVP_PKEY_CTX_set_rsa_keygen_bits(keyCtx, 4096); // RSA 4096
    // variable that will hold both private and public keys
    EVP_PKEY *key = NULL;

    // TODO Make a function for this.
    // generate key
    // EVP_PKEY_keygen(keyCtx, &key);
    // free up key context
    // EVP_PKEY_CTX_free(keyCtx);

    // New method for reading from a private key, this reads from a file.
    // TODO Figure out how to use this, it'll fix a deprecation warning, and it looks like this can read from a file.
    // https://github.com/openssl/openssl/discussions/25034
    // https://ssojet.com/keypair-generation/generate-keypair-using-rsa-in-cpp#generating-the-rsa-key-pair
    // FILE* fp = fopen("private.pem", "wb");

    // To generate a key for this
    /*
    The .pem is normally file set as .crt when I use the command, but either one seems to work.
    openssl req -x509 -newkey rsa:4096 -keyout server.key -out server.pem -days 365 -nodes
    */
    // TODO Move this to a function variable later.
    const char *privateKeyFile = "server.key";
    FILE *fp = fopen(privateKeyFile, "r");
    if (!fp)
    {
        std::cerr << "RSA Private key: " << privateKeyFile << " not found." << std::endl;
        return;
    }

    if (!PEM_read_PrivateKey(fp, &key, NULL, NULL))
    {
        std::cerr << "Failed to read private key" << std::endl;
        return;
    }

    //---------
    // Extract private key as string
    //---------

    unsigned char* privateKey = GetPrivateKey(key);

    // std::cout << "Private Key: \n" << privateKey << std::endl;

    //---------
    // Extract public key as string
    //---------

    unsigned char* publicKey = GetPublicKey(key);
    std::cout << "Public Key: \n" << publicKey << std::endl;
}



/**
 * Testing with RSA.
 * https://gist.github.com/grejdi/9361828
 *
 * TODO Look into using these examples
 * https://github.com/RaymiiOrg/openssl-modern-cpp
 *
 * TODO Make this into multiple functions.
 * TODO Figure out how this is encoding/decoding the keys.
 * TODO Figure out how to encode/decode the encrypted text with Base64.
 *
 * I disabled the below test.
 */
void RsaTest::PublicKeyTesting()
{

    //---------
    // Create private/public key pair
    //---------

    // init RSA context, so we can generate a key pair
    EVP_PKEY_CTX *keyCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    EVP_PKEY_keygen_init(keyCtx);
    EVP_PKEY_CTX_set_rsa_keygen_bits(keyCtx, 4096); // RSA 4096
    // variable that will hold both private and public keys
    EVP_PKEY *key = NULL;
    // generate key
    EVP_PKEY_keygen(keyCtx, &key);
    // free up key context
    EVP_PKEY_CTX_free(keyCtx);

    // EVP_PKEY *key = &CreatePrivateKey();

    //---------
    // Extract private key as string
    // This is now its own function
    //---------

    // create a place to dump the IO, in this case in memory
    BIO *privateBIO = BIO_new(BIO_s_mem());
    // dump key to IO
    PEM_write_bio_PrivateKey(privateBIO, key, NULL, NULL, 0, 0, NULL);
    // get buffer length
    int privateKeyLen = BIO_pending(privateBIO);
    // create char reference of private key length
    unsigned char *privateKeyChar = (unsigned char *)malloc(privateKeyLen);
    // read the key from the buffer and put it in the char reference
    BIO_read(privateBIO, privateKeyChar, privateKeyLen);
    // at this point we can save the private key somewhere

    //---------
    // Extract public key as string
    //---------

    // create a place to dump the IO, in this case in memory
    BIO *publicBIO = BIO_new(BIO_s_mem());

    // dump key to IO
    PEM_write_bio_PUBKEY(publicBIO, key);

    // get buffer length
    int publicKeyLen = BIO_pending(publicBIO);

    // create char reference of public key length
    unsigned char *publicKeyChar = (unsigned char *)malloc(publicKeyLen);

    // read the key from the buffer and put it in the char reference
    BIO_read(publicBIO, publicKeyChar, publicKeyLen);
    // at this point we can save the public somewhere

    // pretend we are pulling the public key from some source and using it
    // to encrypt a message
    unsigned char *rsaPublicKeyChar = publicKeyChar;

    // write char array to BIO
    BIO *rsaPublicBIO = BIO_new_mem_buf(rsaPublicKeyChar, -1);

    // create a RSA object from public key char array
    RSA *rsaPublicKey = NULL;
    PEM_read_bio_RSA_PUBKEY(rsaPublicBIO, &rsaPublicKey, NULL, NULL);

    // create public key
    EVP_PKEY *publicKey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(publicKey, rsaPublicKey);

    //---------
    // Initialize encrypt context
    //---------

    // EVP_CIPHER_CTX *rsaEncryptCtx = (EVP_CIPHER_CTX *) malloc(sizeof(EVP_CIPHER_CTX));
    EVP_CIPHER_CTX *rsaEncryptCtx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(rsaEncryptCtx);

    // variables for where the encrypted secret, length, and IV reside
    unsigned char *ek = (unsigned char *)malloc(EVP_PKEY_size(publicKey));
    int ekLen = 0;
    unsigned char *iv = (unsigned char *)malloc(EVP_MAX_IV_LENGTH);

    //---------
    // generate AES secret, and encrypt it with public key
    EVP_SealInit(rsaEncryptCtx, EVP_aes_256_cbc(), &ek, &ekLen, iv, &publicKey, 1);

    //---------
    // encrypt a message with AES secret
    std::string message = "You can include the standard headers in any order, a standard header more than once, or two or more standard headers that define the same macro or the same type. Do not include a standard header within a declaration. Do not define macros that have the same names as keywords before you include a standard header.";

    const unsigned char *messageChar = (const unsigned char *)message.c_str();
    // length of message
    int messageLen = message.size() + 1;

    // create char reference for where the encrypted message will reside
    unsigned char *encryptedMessage = (unsigned char *)malloc(messageLen + EVP_MAX_IV_LENGTH);

    // the length of the encrypted message
    int encryptedMessageLen = 0;
    int encryptedBlockLen = 0;

    // encrypt message with AES secret
    EVP_SealUpdate(rsaEncryptCtx, encryptedMessage, &encryptedBlockLen, messageChar, messageLen);
    encryptedMessageLen = encryptedBlockLen;

    // finalize by encrypting the padding
    EVP_SealFinal(rsaEncryptCtx, encryptedMessage + encryptedBlockLen, &encryptedBlockLen);
    encryptedMessageLen += encryptedBlockLen;

    // pretend we are decrypting a message we have received using a the private key we have
    unsigned char *rsaPrivateKeyChar = privateKeyChar;

    // write char array to BIO
    BIO *rsaPrivateBIO = BIO_new_mem_buf(rsaPrivateKeyChar, -1);

    // create a RSA object from private key char array
    RSA *rsaPrivateKey = NULL;
    // PEM_read_bio_RSAPrivateKey(rsaPrivateBIO, &rsaPrivateKey, NULL, NULL);

    // New method for reading from a private key, this reads from a file.
    // TODO Figure out how to use this, it'll fix a deprecation warning, and it looks like this can read from a file.
    // https://github.com/openssl/openssl/discussions/25034
    // https://ssojet.com/keypair-generation/generate-keypair-using-rsa-in-cpp#generating-the-rsa-key-pair
    // FILE* fp = fopen("private.pem", "wb");

    // To generate a key for this
    /*
    The .pem is normally file set as .crt when I use the command, but either one seems to work.
    openssl req -x509 -newkey rsa:4096 -keyout server.key -out server.pem -days 365 -nodes
    */
    // TODO Move this to a function variable later.
    const char *privateKeyFile = "server.key";
    FILE *fp = fopen(privateKeyFile, "r");
    if (!fp)
    {
        std::cerr << "RSA Private key: " << privateKeyFile << " not found." << std::endl;
        return;
    }

    if (!PEM_read_PrivateKey(fp, &key, NULL, NULL))
    {
        std::cerr << "Failed to read private key" << std::endl;
        return;
    }

    // create private key
    EVP_PKEY *privateKey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(privateKey, rsaPrivateKey);

    //---------
    // Initialize decrypt context
    //---------

    EVP_CIPHER_CTX *rsaDecryptCtx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(rsaDecryptCtx);

    // decrypt EK with private key, and get AES secretp
    EVP_OpenInit(rsaDecryptCtx, EVP_aes_256_cbc(), ek, ekLen, iv, privateKey);

    // variable for where the decrypted message with be outputed to
    unsigned char *decryptedMessage = (unsigned char *)malloc(encryptedMessageLen + EVP_MAX_IV_LENGTH);

    // the length of the encrypted message
    int decryptedMessageLen = 0;
    int decryptedBlockLen = 0;

    // decrypt message with AES secret
    EVP_OpenUpdate(rsaDecryptCtx, decryptedMessage, &decryptedBlockLen, encryptedMessage, encryptedMessageLen);
    decryptedMessageLen = decryptedBlockLen;
    // finalize by decrypting padding
    EVP_OpenFinal(rsaDecryptCtx, decryptedMessage + decryptedBlockLen, &decryptedBlockLen);
    decryptedMessageLen += decryptedBlockLen;

    // Print out the encrypted message
    // TODO Figure out how to encode the encrypted message, possibly use base64?
    // std::cout << encryptedMessage << std::endl;
    // std::cout << publicBIO << std::endl;
    // std::cout << "Public Key: " << publicKey << std::endl;
    // std::cout << "Private Key: " << privateKey << std::endl;

    // std::cout << "Decrypted message: " << decryptedMessage << std::endl;

    auto encodedMessage = base64::to_base64(encryptedMessage, sizeof(encryptedMessage));
    std::cout << "Encrypted message encoded with base64: " << encodedMessage << std::endl;
}

/**
 * New RSA Testing with library
 * TODO Fix this to work.
 *
 * https://github.com/ParallelEngineering/RSA
 */
// void RsaTest::NewRsaTest()
// {
//     keyPair keys;

//     PublicKey publicKey = keys.getPublicKey();
//     PrivateKey privateKey = keys.getPrivateKey();

//     core::Encryptor encryptor(publicKey);
//     core::Decryptor decryptor(privateKey);

//     const std::string message = "hello";
//     ByteArray ciphertext = encryptor.encrypt(message);
//     std::string plaintext = decryptor.decrypt(ciphertext);

// }

#endif // __linux__