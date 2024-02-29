#ifndef KERNEL_H
#define KERNEL_H

#define ALIGN(x, a)       (((x) + (a) - 1) & ~(a - 1)) // align up. e.g. ALIGN(6,8) = 8, ALIGN(9, 8) = 8.




#endif