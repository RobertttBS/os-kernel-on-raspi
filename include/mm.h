#ifndef __MM_H__
#define __MM_H__

#define PAGE_SHIFT                  (12)
#define MAX_ORDER                   (10)
#define NR_PAGES                    (262144)

#ifndef __ASSEMBLER__

#include "list.h"
#include "stdint.h"

#define PG_buddy                    (0x40) // page frame is free and in buddy system.
#define PG_USED                     (1) // page frame used.
#define PG_KERNEL                   (2) // page frame used by kernel.
#define PG_MMIO                     (3) // page frame used by MMIO.
#define PG_AVAIL                    (4) // page frame is free and not in buddy system. e.g. order 2 page fram [0, 1, 2, 3], flags  = [PG_buddy, PG_AVAIL, PG_AVAIL, PG_AVAIL]

struct page {
    unsigned int flags; // page flags: represent the page state, like compound, dirty, etc.
    unsigned long private; // used to indicate the order of the page in buddy system.
    struct list_head buddy_list; // list_head for buddy system.
};

struct free_area {
    /* At linux kernel, the free_list is an array for different MIGRATE_TYPE. We don't implement it for now. */
    struct list_head free_list;
    unsigned long nr_free; // number of free pages of this order
};

struct zone {
    unsigned long managed_pages; // number of pages managed by buddy system.
    // unsigned long spanned_pages; // number of pages in the zone. not used for now.

    struct free_area free_area[MAX_ORDER]; // the key data structure for buddy system

    /* active_list and inactive_list is for page reclaim, not used for now */
    // struct list_head active_list;
    // struct list_head inactive_list;
};

/* Though we don't have NUMA, use one zone to represent the whole memory space */
extern struct zone zone;

/* page frame: to map the physical memory*/
extern struct page pages[];
extern struct page *mem_map; // mem_map points to pages


#define pfn_to_page(pfn) (mem_map + pfn)
#define page_to_pfn(page) ((unsigned long)((page) - mem_map))
#define pfn_to_phys(pfn) ((unsigned long)(pfn << PAGE_SHIFT))
#define phys_to_pfn(phys) ((unsigned long)(phys >> PAGE_SHIFT))


void buddy_init(void);
struct page *rmqueue(unsigned int order);
void free_one_page(struct page *page, unsigned long pfn, unsigned int order);

/* For print out the buddy system information */
void get_buddy_info(void);

#endif // __ASSEMBLER__
#endif // __MM_H__