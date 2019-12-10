#include "pprint.h"

extern int mbedtls_compat_sgx_printf(const char *fmt, ...);

void dump_buf( const char *title, unsigned char *buf, size_t len )
{
    hexdump(title, buf, len);
}

void string_dump(const char* title, void const* data, size_t len)
{
    unsigned int i;
    
    if (!data)
	return;

    mbedtls_compat_sgx_printf("%s\n", title);
	
    /* ASCII dump */
	for (i = 0; i < len; i++) 	    {
        mbedtls_compat_sgx_printf("%c", ((unsigned char const *)data)[i]);
    }
	mbedtls_compat_sgx_printf("\n");
}

void hexdump(const char* title, void const * data, unsigned int len)
{
    unsigned int i;
    unsigned int r,c;
    
    if (!data)
	return;

    mbedtls_compat_sgx_printf("%s\n", title);
    
    for (r=0,i=0; r<(len/16+(len%16!=0)); r++,i+=16)
    {
        mbedtls_compat_sgx_printf("0x%04X:   ",i); /* location of first byte in line */
	
        for (c=i; c<i+8; c++) /* left half of hex dump */
	    if (c<len)
        	mbedtls_compat_sgx_printf("%02X ",((unsigned char const *)data)[c]);
	    else
		mbedtls_compat_sgx_printf("   "); /* pad if short line */
	
	mbedtls_compat_sgx_printf("  ");
	
	for (c=i+8; c<i+16; c++) /* right half of hex dump */
	    if (c<len)
		mbedtls_compat_sgx_printf("%02X ",((unsigned char const *)data)[c]);
	    else
		mbedtls_compat_sgx_printf("   "); /* pad if short line */
	
	mbedtls_compat_sgx_printf("   ");
	
	for (c=i; c<i+16; c++) /* ASCII dump */
	    if (c<len)
		if (((unsigned char const *)data)[c]>=32 &&
		    ((unsigned char const *)data)[c]<127)
		    mbedtls_compat_sgx_printf("%c",((char const *)data)[c]);
		else
		    mbedtls_compat_sgx_printf("."); /* put this for non-printables */
	    else
		mbedtls_compat_sgx_printf(" "); /* pad if short line */
	
	mbedtls_compat_sgx_printf("\n");
    }
}