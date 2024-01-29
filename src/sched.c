#include "delays.h"
#include "sched.h"

char kstack_pool[NR_TASKS][KSTACK_SIZE];
struct task_struct task_pool[NR_TASKS];

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
    for (int i = 0; i < NR_TASKS; i++) {
        task_pool[i].state = TASK_STOPPED;
        task_pool[i].task_id = i;
    }

    // TODO: Understand how to deal with the task[0] (kernel). At linux 0.11, it is a special task.
    task_pool[0].state = TASK_RUNNING;
    task_pool[0].priority = 1000;
    task_pool[0].counter = 1000;
    update_current(&task_pool[0]);
}

void context_switch(struct task_struct *next)
{
    struct task_struct *prev = current; // the current task_struct address
    update_current(next);
    next->counter--; // Currently, I update counter here. But i think it should be updated in timer interrupt at labs behind.
    switch_to(&prev->tss, &next->tss);
}

void demo_task1()
{
    while (1) {
        uart_puts("demo task1\n");
        wait_sec(1);
        schedule();
        // context_switch(&task_pool[2]); // for requirement 1-3
    }
}

void demo_task2()
{
    while (1) {
        uart_puts("demo task2\n");
        wait_sec(1);
        schedule();
        // context_switch(&task_pool[1]); // for requirement 1-3
    }
}

void print_task(int i)
{
    uart_puts("task pool: ");
    uart_hex(i);
    uart_puts(" state: ");
    uart_hex(task_pool[i].state);
    uart_puts(" counter: ");
    uart_hex(task_pool[i].counter);
    uart_send('\n');
}

void privilege_task_create(void (*func)(), unsigned int priority)
{
    int i;

    for (i = 1; i < NR_TASKS; i++) {
        if (task_pool[i].state == TASK_STOPPED) {
            task_pool[i].state = TASK_RUNNING;
            task_pool[i].priority = priority;
            task_pool[i].counter = priority;
            task_pool[i].tss.lr = (uint64_t) func;
            task_pool[i].tss.sp = (uint64_t) &kstack_pool[i][KSTACK_TOP];
            task_pool[i].tss.fp = (uint64_t) &kstack_pool[i][KSTACK_TOP];
            uart_puts("put function at task ");
            uart_hex(i);
            uart_send('\n');
            break;
        }
    }

    if (i == NR_TASKS) {
        uart_puts("task pool is full, can't create a new task.\n");
        while (1);
    }
}

/* Select the task with the highest counter to run. If all tasks' counter is 0, then update all tasks' counter with their priority. */
void schedule()
{
    int i, c = 0, next = 0;
    while (1) {
        for (i = 1; i < NR_TASKS; i++) {
            if (task_pool[i].state == TASK_RUNNING && task_pool[i].counter > c) {
                c = task_pool[i].counter, next = i;
            }
        }
        if (c)
            break;
        for (i = 1; i < NR_TASKS; i++) // prevent return to task0
            if (task_pool[i].state == TASK_RUNNING)
                task_pool[i].counter = (task_pool[i].counter >> 1) + task_pool[i].priority;
    }
    context_switch(&task_pool[next]);
}


void sched_init()
{
    privilege_task_create(demo_task1, 100);
    privilege_task_create(demo_task2, 100);
    schedule();
}