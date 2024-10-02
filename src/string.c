/**
 * Helper function to convert ASCII octal number into binary
 * s string
 * n number of digits
 */
int oct2bin(char *s, int n)
{
    int r=0;
    while(n-- > 0) {
        r <<= 3;
        r += *s++ - '0';
    }
    return r;
}

int hex2bin(char *s, int n)
{
    int r = 0;
    while (n-- > 0) {
        r <<= 4;
        if (*s >= '0' && *s <= '9')
            r += *(s++) - '0';
        else if (*s >= 'a' && *s <= 'f')
            r += *s++ - 'a' + 10;
        else if (*s >= 'A' && *s <= 'F')
            r += *s++ - 'A' + 10;
        else
            return -1;
    }
    return r;
}

/* If a == b, return 0. Else return non-zero value. */
int strcmp(const char *a, const char *b)
{
    while (*a) {
        if (*a != *b)
            break;
        a++;
        b++;
    }
    return *(const unsigned char *)a - *(const unsigned char *)b;
}

/* if s1 == s2, return 0. Else return non-zero value */
int memcmp(void *s1, void *s2, int n)
{
    const char *a = s1;
    const char *b = s2;
    if (n != 0) {
        do {
            if (*a++ != *b++)
                return (*--a - *--b);
        } while (--n != 0);
    }
    return 0;
}

int strlen(const char *str)
{
    const char *s;

    for (s = str; *s; ++s);
    return (s - str);
}

char *strcpy(char *dst, const char *src)
{
    char *ret = dst;

    for (; (*dst = *src); src++, dst++);
    return ret;
}

char *strcat(char *dst, const char *src)
{
    char *save = dst;

    for (; *dst; dst++);
    while ((*dst++ = *src++));
    return save;
}

int atoi(char *s)
{
    int n = 0, sign = 1;
    if (*s == '-') {
        sign = -1;
        s++;
    }
    while (*s >= '0' && *s <= '9') {
        n = n * 10 + (*s - '0');
        s++;
    }
    return n * sign;
}

void *memset(void *str, int c, unsigned long n) {
    char *ptr = (char *)str;

    while (n-- > 0)
        *ptr++ = (char)c;
    return str;
}

void *memcpy(void *dest, const void *src, unsigned long n)
{
    char *d = dest;
    const char *s = src;

    while (n--)
        *d++ = *s++;
    return dest;
}