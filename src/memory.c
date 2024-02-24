/**
 * memory.c - a collection of memory operation functions
 * The memory management functions are implemented at mm.c.
 * There are some memory functions that related to memory operation.
*/

int memcmp(void *s1, void *s2, int n)
{
    unsigned char *a=s1,*b=s2;
    while(n-->0){ if(*a!=*b) { return *a-*b; } a++; b++; }
    return 0;
}

void *
memset(void *dest, int c, int n)
{
    char *d = dest;
    while (n-- > 0)
        *d++ = c;
    return dest;
}

void *
memzero(void *dest, int n)
{
    return memset(dest, 0, n);
}

void *
memcpy(void *dest, const void *src, int n)
{
    char *d = dest;
    const char *s = src;
    while (n-- > 0)
        *d++ = *s++;
    return dest;
}