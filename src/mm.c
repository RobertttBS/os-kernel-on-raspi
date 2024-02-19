#include "mm.h"

struct page pages[PAGE_FRAMES_NUM];

#define LOW_MEM                 (0x0)
#define PAGING_MEMORY           (1 << 30)
#define PAGING_PAGES            (PAGING_MEMORY >> 12)
#define MAP_NR(addr)            ((addr - LOW_MEM) >> 12) /* Physical address to page frame number */

/* mem_map is used by Linux 0.11 to keep track the number being referenced of a page frame, but I do not use it for now */
// unsigned char mem_map[PAGING_PAGES] = {0,};

extern unsigned char _start;
extern unsigned char _end;

unsigned long page_alloc(void)
{
    for (int i = 0; i < PAGE_FRAMES_NUM; i++) {
        if (pages[i].flags == 0) {
            pages[i].flags = 1;
            return i << 12;
        }
    }
    return 0;
}

/* Just supposed that the input parameter is physical memory address*/
void page_free(unsigned long addr)
{
    if (pages[MAP_NR(addr)].flags--)
        return;
    /* We should not be here, because there's no process using this page */
    while (1);
    
}

/* Initialize the page frame struct */
void page_frame_init(void)
{
    unsigned long kernel_start = ((unsigned long) &_start & ~(0xffff000000000000));
    unsigned long kernel_end = ((unsigned long) &_end & ~(0xffff000000000000));
    /* 0x000 ~ 0x3fff are for page table (PGD, PUD, PTE...)*/
    for (int i = 0; i < PAGE_FRAMES_NUM; i++) {
        // unsigned long addr = i << 12;
        if (i < 4) {
            pages[i].flags = 1; // The first four page frames are for page tables.
        } else if ((i << 12) >= kernel_start && (i << 12) <= kernel_end) {
            pages[i].flags = 1; // for kernel usage
        } else {
            pages[i].flags = 0; // for user usage
        }
    }
}

/* Virtual address to physical address (table walk) */
unsigned long va_to_pa(unsigned long va)
{
    unsigned long pgd_index = (va >> 39) & 0x1ff;
    unsigned long pud_index = (va >> 30) & 0x1ff;
    unsigned long pmd_index = (va >> 21) & 0x1ff;
    unsigned long pte_index = (va >> 12) & 0x1ff;

    unsigned long *pgd = (unsigned long *) MMU_PGD_ADDR;
    unsigned long *pud = (unsigned long *) (pgd[pgd_index] & 0x0000fffffffff000);
    unsigned long *pmd = (unsigned long *) (pud[pud_index] & 0x0000fffffffff000);
    unsigned long *pte = (unsigned long *) (pmd[pmd_index] & 0x0000fffffffff000);

    /* pte is a table entry or a 2 MB block entry*/
    if ((*pte & 0x1) && (*pte & 0x2)) // Check if the page is a table entry
        return (pte[pte_index] & 0x0000fffffffff000) | (va & 0xfff);
    else // 2 MB block entry
        return (*pte & 0x0000ffffffe00000) | (va & 0x1fffff);
}

/* Virtual address to PFN (page frame number) */
int va_to_pfn(unsigned long va)
{
    return (va_to_pa(va)) >> 12;
}

/* Not sure whether we need this function. */
unsigned long pfn_to_pa(int pfn)
{
    return pfn << 12;
}

/* Setup PGD, PUD, PMD, PTE. (0x0000 ~ 0x3fff)*/
void setup_page_table(void)
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