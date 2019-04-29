#ifndef MBEDTLS_THREADING_ALT_H
#define MBEDTLS_THREADING_ALT_H

#include <mbedtls/threading.h>
#include <sgx_thread.h>

typedef struct mbedtls_threading_mutex_t
{
    sgx_thread_mutex_t mutex;
    char is_valid;
} mbedtls_threading_mutex_t;

void threading_mutex_init_sgx( mbedtls_threading_mutex_t *mutex );
void threading_mutex_free_sgx( mbedtls_threading_mutex_t *mutex );
int threading_mutex_lock_sgx( mbedtls_threading_mutex_t *mutex );
int threading_mutex_unlock_sgx( mbedtls_threading_mutex_t *mutex );

/*
 * With SGX we can statically initialize mutexes
 */
#define MUTEX_INIT  = { SGX_THREAD_MUTEX_INITIALIZER, 1 }

#endif /* MBEDTLS_THREADING_ALT_THREADING_ALT_H */
