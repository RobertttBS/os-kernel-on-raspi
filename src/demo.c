#include "sched.h"
#include "uart.h"
#include "exec.h"
#include "syscall.h"
#include "delays.h"

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

void timer_task1()
{
    while (1) {
        uart_puts("kernel timer task1.\n");
        wait_sec(10);
    }
}

void timer_task2()
{
    while (1) {
        uart_puts("kernel timer task2.\n");
        wait_sec(10);
    }
}

void user_task1()
{
    while (1) {
        uart_puts("user task1\n");
        int count = 1000000000;
        while (count--);
    }
}

void user_task2()
{
    while (1) {
        uart_puts("user task2\n");
        int count = 1000000000;
        while (count--);
    }
}

void demo_do_exec1()
{
    do_exec(user_task1);
}

void demo_do_exec2()
{
    do_exec(user_task2);
}

void delay()
{
    long count = 1000000; // 1000000000 can see the effect of multi-tasking, or I should increase the frequency of timer interrupt.
    while (count--);
}

void foo(){
    int tmp = 5;
    printf("Task %d after exec, tmp address 0x%x, tmp value %d\n", get_taskid(), &tmp, tmp);
    exit(0);
}

/* At user space, not allowed to access timer */
void test() {
    printf("Into test()\n");
    int cnt = 1;
    if (fork() == 0) {
        fork();
        delay();
        fork();
        while(cnt < 10) {
            printf("Task id: %d, cnt: %d\n", get_taskid(), cnt);
            delay();
            ++cnt;
        }
        printf("Child task %d done before exit\n", get_taskid());
        exit(0);
        printf("Should not be printed\n");
    } else {
        printf("Task %d before exec, cnt address 0x%x, cnt value %d\n", get_taskid(), &cnt, cnt);
        exec(foo);
    }
}

void user_test()
{
    do_exec(test);
}

void fork_test()
{
    printf("\nFork Test, pid %d\n", get_taskid());
    int cnt = 1;
    int ret = 0;
    if ((ret = fork()) == 0) {
        long long cur_sp;
        asm volatile("mov %0, sp": "=r"(cur_sp));
        printf("first child pid: %d, cnt: %d, ptr: %x, sp: %x\n", get_taskid(), cnt, &cnt, cur_sp);
        ++cnt;

        if ((ret = fork()) != 0) {
            asm volatile("mov %0, sp": "=r"(cur_sp));
            printf("first child pid: %d, cnt: %d, ptr: %x, sp: %x\n", get_taskid(), cnt, &cnt, cur_sp);
        } else {
            while (cnt < 5) {
                asm volatile("mov %0, sp": "=r"(cur_sp));
                printf("second child pid: %d, cnt: %d, ptr: %x, sp: %x\n", get_taskid(), cnt, &cnt, cur_sp);
                delay();
                ++cnt;
            }
        }
        exit(0);
    } else {
        printf("parent here, pid %d, child %d\n", get_taskid(), ret);
        exit(0);
    }
}

void demo_fork_test()
{
    do_exec(fork_test);
}