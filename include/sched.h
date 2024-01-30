#ifndef __SCHED_H
#define __SCHED_H

#include "uart.h"
#include "stdint.h"

#define NR_TASKS 64
#define FIRST_TASK task_pool[0]
#define LAST_TASK task_pool[NR_TASKS - 1]
#define KSTACK_SIZE 4096
#define KSTACK_TOP (KSTACK_SIZE - 16)
#define USTACK_SIZE 4096
#define USTACK_TOP (USTACK_SIZE - 16)
#define PAGE_SIZE 4096
#define current get_current()

/* 
// Codes below is used in linux 0.11. But useless in OSDI (hierarychy difference)
#define INIT_TASK \
{ \
    .task_id = 0, \
    .state = TASK_RUNNING, \
    .priority = 1, \
    .counter = 1, \
    .tss = { \
        .x19 = 0, \
        .x20 = 0, \
        .x21 = 0, \
        .x22 = 0, \
        .x23 = 0, \
        .x24 = 0, \
        .x25 = 0, \
        .x26 = 0, \
        .x27 = 0, \
        .x28 = 0, \
        .fp = 0, \
        .lr = 0, \
        .sp = 0, \
        .pc = 0, \
    }, \
}
*/

enum task_state {
    TASK_RUNNING,
    TASK_INTERRUPTIBLE,
    TASK_UNINTERRUPTIBLE,
    TASK_ZOMBIE,
    TASK_STOPPED,
};

struct task_state_segment { // cpu context
    uint64_t x19;
    uint64_t x20;
    uint64_t x21;
    uint64_t x22;
    uint64_t x23;
    uint64_t x24;
    uint64_t x25;
    uint64_t x26;
    uint64_t x27;
    uint64_t x28;
    uint64_t fp; // x29, frame pointer
    uint64_t lr; // x30, link register
    uint64_t sp;
    uint64_t pc;
};

struct task_struct {
    unsigned int task_id;
    enum task_state state;
    unsigned int priority;
    unsigned int counter;
    struct task_state_segment tss; // cpu context
};

extern struct task_struct task_pool[NR_TASKS];
extern char kstack_pool[NR_TASKS][KSTACK_SIZE];
extern char ustack_pool[NR_TASKS][USTACK_SIZE];

/* function in schedule.S */
extern struct task_struct *get_current();
extern void switch_to(struct task_state_segment *prev, struct task_state_segment *next);
extern void update_current(struct task_struct *next);

/* function in sched.c */
void task_init(); // task init is to setup everything about task and the first task.
void sched_init();
void context_switch(struct task_struct *next);
void privilege_task_create(void (*func)(), unsigned int priority);
void do_exec(void (*func)());
void schedule();


#endif // __SCHED_H