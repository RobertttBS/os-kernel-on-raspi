#ifndef __MEMORY_H__
#define __MEMORY_H__

int memcmp(void *s1, void *s2, int n);
void *memset(void *dest, int c, int n);
void *memzero(void *dest, int n);
void *memcpy(void *dest, void *src, int n);


#endif // __MEMORY_H__