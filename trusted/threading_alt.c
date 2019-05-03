#include <sgx_thread.h>

#include <mbedtls/threading.h>
#include <threading_alt.h>

void threading_mutex_init_sgx(mbedtls_threading_mutex_t *mutex)
{
    if (mutex == NULL)
        return;

    mutex->is_valid = sgx_thread_mutex_init(&mutex->mutex, NULL) == 0;
}

void threading_mutex_free_sgx(mbedtls_threading_mutex_t *mutex)
{
    if (mutex == NULL || !mutex->is_valid)
        return;

    (void)sgx_thread_mutex_destroy(&mutex->mutex);
    mutex->is_valid = 0;
}

int threading_mutex_lock_sgx(mbedtls_threading_mutex_t *mutex)
{
    if (mutex == NULL || !mutex->is_valid)
        return (MBEDTLS_ERR_THREADING_BAD_INPUT_DATA);

    if (sgx_thread_mutex_lock(&mutex->mutex) != 0)
        return (MBEDTLS_ERR_THREADING_MUTEX_ERROR);

    return (0);
}

int threading_mutex_unlock_sgx(mbedtls_threading_mutex_t *mutex)
{
    if (mutex == NULL || !mutex->is_valid)
        return (MBEDTLS_ERR_THREADING_BAD_INPUT_DATA);

    if (sgx_thread_mutex_unlock(&mutex->mutex) != 0)
        return (MBEDTLS_ERR_THREADING_MUTEX_ERROR);

    return (0);
}

void (*mbedtls_mutex_init)( mbedtls_threading_mutex_t * ) = threading_mutex_init_sgx;
void (*mbedtls_mutex_free)( mbedtls_threading_mutex_t * ) = threading_mutex_free_sgx;
int (*mbedtls_mutex_lock)( mbedtls_threading_mutex_t * ) = threading_mutex_lock_sgx;
int (*mbedtls_mutex_unlock)( mbedtls_threading_mutex_t * ) = threading_mutex_unlock_sgx;

/*
 * With SGX we can statically initialize mutexes
 */
#define MUTEX_INIT  = { SGX_THREAD_MUTEX_INITIALIZER, 1 }