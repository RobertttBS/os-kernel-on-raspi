#include "mm.h"


void mem_setup_page_table(void)
{
    unsigned long *pgd = (unsigned long *) MMU_PGD_ADDR;
    unsigned long *pud = (unsigned long *) MMU_PUD_ADDR;
    pgd[0] = (unsigned long) MMU_PUD_ADDR | BOOT_PGD_ATTR;
    

    pud[0] = (unsigned long) (0x00000000 | PUD_NORMAL_BLOCK_ATTR);
    pud[1] = (unsigned long) (0x40000000 | PUD_DEVICE_BLOCK_ATTR);
    return;
}