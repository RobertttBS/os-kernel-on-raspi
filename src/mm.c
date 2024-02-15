#include "mm.h"

void setup_page_table()
{
    unsigned long *pgd = (unsigned long *) MMU_PGD_ADDR;
    unsigned long *pud = (unsigned long *) MMU_PUD_ADDR;
    unsigned long *pmd = (unsigned long *) MMU_PMD_ADDR;
    unsigned long *pte = (unsigned long *) MMU_PTE_ADDR;

    /* Setup PGD */
    pgd[0] = (unsigned long) MMU_PUD_ADDR | BOOT_PGD_ATTR;

    /* Setup PUD */
    pud[0] = (unsigned long) MMU_PMD_ADDR | NORMAL_PAGE_ATTR;
    pud[1] = (unsigned long) 0x40000000 | DEVICE_BLOCK_ATTR;

    /* Setup PMD, each entry points to 2 MB block*/
    pmd[0] = (unsigned long) MMU_PTE_ADDR | NORMAL_PAGE_ATTR; // point pmd[0] to first pte table
    for (int i = 1; i < 512; i++) {
        /* i << 21 equals 2 MB */
        unsigned long addr = i << 21;
        if (addr >= PERIPH_MMIO_BASE)
            pmd[i] = (unsigned long) addr | DEVICE_BLOCK_ATTR;
        else
            pmd[i] = (unsigned long) addr | NORMAL_BLOCK_ATTR;
    }

    /* If we use PTE, we must have 512 PTE table, each entry points to 4 KB block*/
    /* Setup the first PTE table (Only the first pte table, pointed by pmd[0]) */
    for (int i = 0; i < 512; i++) {
        unsigned long addr = i << 12;
        /* L3 table entry can use `0b11` as page attribute */
        if (addr >= PERIPH_MMIO_BASE)
            pte[i] = (unsigned long) addr | DEVICE_PAGE_ATTR;
        else
            pte[i] = (unsigned long) addr | NORMAL_PAGE_ATTR;
    }

    asm volatile("msr ttbr0_el1, %0" : : "r" (MMU_PGD_ADDR));
    asm volatile("msr ttbr1_el1, %0" : : "r" (MMU_PGD_ADDR));
    return;
}