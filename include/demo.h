#ifndef __DEMO_H__
#define __DEMO_H__

void demo_task1(void);
void demo_task2(void);
void timer_task1(void);
void timer_task2(void);

/* demo_do_exec are functions in kernel mode */
void demo_do_exec1(void);
void demo_do_exec2(void);

void user_test(void);

void demo_fork_test(void);

void test_buddy(void);
void test_slab(void);
void test_memblock(void);

#endif