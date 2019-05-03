#ifndef MBEDTLS_THREADING_ALT_H
#define MBEDTLS_THREADING_ALT_H

#undef MBEDTLS_THREADING_ALT

#include <mbedtls/threading.h>
#include <sgx_thread.h>

typedef struct mbedtls_threading_mutex_t
{
    sgx_thread_mutex_t mutex;
    char is_valid;
} mbedtls_threading_mutex_t;

#endif /* MBEDTLS_THREADING_ALT_THREADING_ALT_H */
