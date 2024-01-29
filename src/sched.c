#include "delays.h"
#include "sched.h"

char kstack_pool[TASK_POOL_SIZE][KSTACK_SIZE];
struct task_struct task_pool[TASK_POOL_SIZE];

/* 
// Codes below is the structure used in linux 0.11. But useless in osdi (hierarychy difference)
union task_union {
    struct task_struct task;
    char stack[PAGE_SIZE];
};
static union task_union init_task = {INIT_TASK, };
struct task_struct *current = &(init_task.task);
*/

void task_init()
{
    for (int i = 0; i < TASK_POOL_SIZE; i++) {
        task_pool[i].status = TASK_STOPPED;
        task_pool[i].task_id = i;
    }

    task_pool[0].status = TASK_RUNNING;
    update_current(&task_pool[0]);
}

void context_switch(struct task_struct *next)
{
    struct task_struct *prev = current; // the current task_struct address
    update_current(next);
    switch_to(&prev->tss, &next->tss);
}

void demo_task1()
{
    while (1) {
        uart_puts("demo task1\n");
        wait_sec(1);
        // schedule();
        context_switch(&task_pool[2]);
    }
}

void demo_task2()
{
    while (1) {
        uart_puts("demo task2\n");
        wait_sec(1);
        // schedule();
        context_switch(&task_pool[1]);
        // uart_puts("task2 -> task1\n");
    }
}

void print_task(int i)
{
    uart_puts("task pool: ");
    uart_hex(i);
    uart_puts(" status: ");
    uart_hex(task_pool[i].status);
    uart_send('\n');
}

void privilege_task_create(void (*func)())
{
    int i;

    for (i = 0; i < TASK_POOL_SIZE; i++) {
        // print_task(i);
        if (task_pool[i].status == TASK_STOPPED) {
            task_pool[i].status = TASK_RUNNING;
            task_pool[i].tss.lr = (uint64_t) func;
            task_pool[i].tss.sp = (uint64_t) &kstack_pool[i][KSTACK_TOP];
            task_pool[i].tss.fp = (uint64_t) &kstack_pool[i][KSTACK_TOP];
            break;
        }
    }

    if (i == TASK_POOL_SIZE) {
        uart_puts("task pool is full\n");
        while (1);
    }

    // context_switch(&task_pool[i]);
}

void schedule()
{
    context_switch(&task_pool[1]);
    // int i;
    // for (i = 1; i < TASK_POOL_SIZE; i++) {
    //     // print_task(i);
    //     if (task_pool[i].status == TASK_RUNNING && i != current->task_id) {
    //         break;
    //     }
    // }
    // if (i == TASK_POOL_SIZE) {
    //     uart_puts("no task to run\n");
    //     return;
    // }
    // context_switch(&task_pool[i]);
}


void sched_init()
{
    privilege_task_create(demo_task1);
    privilege_task_create(demo_task2);
    schedule();
}