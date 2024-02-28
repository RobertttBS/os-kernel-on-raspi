#ifndef STDLIB_H
#define STDLIB_H


#ifndef __SIZE_T__
#define __SIZE_T__
typedef unsigned long size_t;
#endif // __SIZE_T__

#define BASE ((volatile unsigned int*)(0x60000))
#define LIMIT ((volatile unsigned int*)(0x7FFFF))

extern volatile unsigned long available;

void *simple_malloc(size_t size);
int return_available();

#endif