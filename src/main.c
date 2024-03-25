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
#include "demo.h"
#include "memblock.h"
#include "fs.h"

#define CMD_LEN 128

extern void move_to_user_mode(void); // defined in exception_.S
extern void core_timer_enable(void); // defined in timer_.S

void main()
{
    shell_init();
    memblock_init(); // memblock init should be called before any memory allocation.
    
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

    mm_init();
    vfs_init();



    while(1) {
        uart_puts("# ");
        char cmd[CMD_LEN];
        shell_input(cmd);
        shell_controller(cmd);
    }
}
