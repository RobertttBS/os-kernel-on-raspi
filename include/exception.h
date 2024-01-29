#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include "uart.h"

#define CORE0_IRQ_SOURCE ((volatile unsigned int*)(0x40000060))

#define IRQ_BASE        MMIO_BASE+0x0000B000

#define IRQ_BASIC       ((volatile unsigned int*)(IRQ_BASE+0x00000200)) // irq basic pending
#define IRQ_PEND1       ((volatile unsigned int*)(IRQ_BASE+0x00000204)) // irq pending 1
#define IRQ_PEND2       ((volatile unsigned int*)(IRQ_BASE+0x00000208)) // irq pending 2
#define IRQ_FIQ         ((volatile unsigned int*)(IRQ_BASE+0x0000020C)) // irq FIQ control
#define IRQ_ENABLE1     ((volatile unsigned int*)(IRQ_BASE+0x00000210)) // irq enable 1
#define IRQ_ENABLE2     ((volatile unsigned int*)(IRQ_BASE+0x00000214)) // irq enable 2
#define IRQ_ENABLE_BASIC ((volatile unsigned int*)(IRQ_BASE+0x00000218)) // irq enable basic
#define IRQ_DISABLE1    ((volatile unsigned int*)(IRQ_BASE+0x0000021C)) // irq disable 1
#define IRQ_DISABLE2    ((volatile unsigned int*)(IRQ_BASE+0x00000220)) // irq disable 2
#define IRQ_DISABLE_BASIC ((volatile unsigned int*)(IRQ_BASE+0x00000224)) // irq disable basic

static inline void move_to_user_mode()
{
    asm volatile("bl from_el1_to_el0    \n\t");
}

static inline void enable_interrupt()
{
    asm volatile("msr DAIFClr, 0xf");
}

static inline void disable_interrupt()
{
    asm volatile("msr DAIFSet, 0xf");
}

void print_current_el();

#endif // __EXCEPTION_H__