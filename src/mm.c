#include "mm.h"


void setup_page_table()
{
    unsigned long *pgd = (unsigned long *) MMU_PGD_ADDR;
    unsigned long *pud = (unsigned long *) MMU_PUD_ADDR;
    unsigned long *pmd = (unsigned long *) MMU_PMD_ADDR;
    unsigned long *pte = (unsigned long *) MMU_PTE_ADDR;

    pgd[0] = (unsigned long) MMU_PUD_ADDR | BOOT_PGD_ATTR;

    pud[0] = (unsigned long) MMU_PMD_ADDR | NORMAL_PAGE_ATTR;
    for (int i = 0; i < 512; i++) {
        /* i << 21 equals 2 MB */
        pmd[i] = (i << 21) | NORMAL_BLOCK_ATTR;
    }

    pud[1] = (unsigned long) 0x40000000 | BOOT_PUD_ATTR;

    asm volatile("msr ttbr0_el1, %0" : : "r" (MMU_PGD_ADDR));
    asm volatile("msr ttbr1_el1, %0" : : "r" (MMU_PGD_ADDR));
    return;
}