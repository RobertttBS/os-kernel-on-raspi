#ifndef __STDLIB_H__
#define __STDLIB_H__


#ifndef __SIZE_T__
typedef unsigned long size_t;
#define __SIZE_T__
#endif // __SIZE_T__

#define BASE ((volatile unsigned int*)(0x60000))
#define LIMIT ((volatile unsigned int*)(0x7FFFF))

extern volatile unsigned long available;

void *simple_malloc(size_t size);
int return_available();

#endif