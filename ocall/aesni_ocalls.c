unsigned int ocall_get_cpuid()
{
    static int done = 0;
    static unsigned int c = 0;

    if (!done)
    {
        asm("movl  $1, %%eax   \n\t"
            "cpuid             \n\t"
            : "=c"(c)
            :
            : "eax", "ebx", "edx");
        done = 1;
    }

    return c;
}
