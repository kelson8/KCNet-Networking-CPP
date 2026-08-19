#pragma once

#ifdef __linux__

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <openssl/evp.h>
#include <openssl/pem.h>

class OpensslTest
{
public:
    static void init_openssl(void);

    int create_tcp_socket(int port);
    int create_tcp_client(const char* server_ip, int port);

    SSL_CTX* create_client_ssl_context();

    
    SSL_CTX* create_ssl_context();
    void configure_ssl_context(SSL_CTX* ctx, const char* publicKeyFile, const char* privateKeyFile);

    static void openssl_cleanup(void);

    // void PublicKeyTesting();
private:
    // EVP_PKEY &CreatePrivateKey();
};

#endif // __linux__