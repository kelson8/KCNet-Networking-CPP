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
};

#endif // __linux__