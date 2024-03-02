#ifndef __STRING_H__
#define __STRING_H__


int oct2bin(char *s, int n);
int hex2bin(char *s, int n);
int strcmp(const char *a, const char *b);
int memcmp(void *s1, void *s2, int n);
int strlen(char *str);
int strcpy(char *dst, char *src);
int atoi(char *s);


#endif