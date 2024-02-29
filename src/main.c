#include "uart.h"
#include "power.h"
#include "shell.h"
#include "mbox.h"
#include "stdlib.h"
#include "dtb.h"
#include "initrd.h"
#include "exception.h"
#include "sched.h"
#include "timer.h"
#include "mm.h"
#include "slab.h"

#define CMD_LEN 128

extern void move_to_user_mode(void); // defined in exception_.S
extern void core_timer_enable(void); // defined in timer_.S

void main()
{
    shell_init();
    get_memory_info();
    // uart_async_init();
    fdt_init();
    fdt_traverse(initramfs_callback);

    timer_init();
    // core_timer_enable(); // User have to use `timer_on` to enable timer before `set_timeout`.

    print_current_el(); // read the current level from system register.

    // task_init();

    // disable_interrupt();

    // sched_init(); // start schedule

    /* Switch to el0 before running shell. Unnessasary in lab 4*/
    // move_to_user_mode();

    buddy_init();

    // get_buddy_info();
    // struct page *test_page1 = __alloc_pages(0);
    // printf("physical address: %x\n", test_page1, pfn_to_phys(page_to_pfn(test_page1)));
    // get_buddy_info();
    
    // struct page *test_page2 = __alloc_pages(1);
    // printf("physical address: %x\n", test_page2, pfn_to_phys(page_to_pfn(test_page2)));
    // get_buddy_info();

    // free_one_page(test_page1, page_to_pfn(test_page1), 0);
    // free_one_page(test_page2, page_to_pfn(test_page2), 1);
    // get_buddy_info();

    // struct page *test_page3 = __alloc_pages(8);
    // printf("physical address: %x\n", test_page3, pfn_to_phys(page_to_pfn(test_page3)));
    // get_buddy_info();

    slab_init();

    void *test_slab1 = get_object(9);
    printf("test_slab1: %x\n", test_slab1);
    void *test_slab2 = get_object(10);
    printf("test_slab2: %x\n", test_slab2);
    void *test_slab3 = get_object(11);
    printf("test_slab3: %x\n", test_slab3);

    free_object(test_slab2);
    void *test_slab4 = get_object(12);
    printf("test_slab4: %x\n", test_slab4);

    while(1) {
        uart_puts("# ");
        char cmd[CMD_LEN];
        shell_input(cmd);
        shell_controller(cmd);
    }
}
