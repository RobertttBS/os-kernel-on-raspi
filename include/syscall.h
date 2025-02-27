#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include "stddef.h"

#define SYS_GET_TASKID    0
#define SYS_UART_READ     1
#define SYS_UART_WRITE    2
#define SYS_EXEC          3
#define SYS_FORK          4
#define SYS_EXIT          5
#define SYS_MBOX_CALL     6
#define SYS_KILL          7
#define SYS_SIGNAL        8
#define SYS_SIGKILL       9
#define SYS_SIGRETURN    10
#define SYS_OPEN         11
#define SYS_CLOSE        12
#define SYS_WRITE        13
#define SYS_READ         14
#define SYS_MKDIR        15
#define SYS_MOUNT        16
#define SYS_CHDIR        17

#define SYSCALL_NUM      (18)
#define SYSCALL_SUCCESS  (0)
#define SYSCALL_ERROR    (-1)

#ifndef O_CREAT
#define O_CREAT          (100)
#endif

/** 
 * Because include syscall.h will copy all the content in header file,
 * considering the portability, we should split the content in syscall.h.
 * Content above is the content for systemcall.S and content below is for syscall.c.
 * */
#ifndef __ASSEMBLER__ // Content below is not included in assembly file. (__ASSEMBLY__ is defined in assembly file)

#include "stdlib.h"
#include "stdint.h"

/* ref: linux pt_regs */
struct trapframe {
    uint64_t x[31];
    uint64_t sp; // sp_el0, user space stack pointer
    uint64_t pstate; // spsr_el1
    uint64_t pc; // elr_el1, it's the return address before entering kernel mode. (el0 -> el1 or el1 -> el1)
};

/* for systemcall.S, user space function */
/* system call: get current task id. */
extern int get_taskid();
/* system call: uart_read(buf, size). Use uart_getc() to fill the buf for given size. */
extern size_t uart_read(char *buf, size_t size);
/* system call: uart_write(buf, size). Output the contents of the buf for given size. */
extern size_t uart_write(const char *buf, size_t size);
/* system call: exec(func). For user space to execute the function. */
extern int exec(const char *name, char *const argv[]);
/* system call: For user space to fork current task. */
extern int fork();
/* system call: exit(status). */
extern void exit(int status);
/* system call: int mbox_call(unsigned char ch, unsigned int *mbox) */
extern int mailbox_call(unsigned char ch, unsigned int *mbox);
/* system call: void kill(int pid) */
extern void kill(int pid);
/* system call to return kernel from signal handling. */
extern void sigreturn(void);
/* system call to open a file and return file descriptor. */
extern int open(const char *pathname, int flags);
/* system to close file according to file descriptor. */
extern int close(int fd);
/* system call to write file, remember to return read size or error code */
extern long write(int fd, const void *buf, unsigned long count);
/* system call to read file, remember to return read size or error code */
extern long read(int fd, void *buf, unsigned long count);
/* system call to make directory, you can ignore mode, since there is no access control */
extern int mkdir(const char *pathname, unsigned mode);
/* system call to mount file system, you can ignore arguments other than target (where to mount) and filesystem (fs name) */
extern int mount(const char *src, const char *target, const char *filesystem, unsigned long flags, const void *data);
/* system call to change current working directory. */
extern int chdir(const char *path);

/* for syscall.c, kernel space handler function */
int sys_get_taskid(struct trapframe *trapframe);
int sys_uart_read(struct trapframe *trapframe);
int sys_uart_write(struct trapframe *trapframe);
int sys_exec(struct trapframe *trapframe);
int sys_fork(struct trapframe *trapframe);
int sys_exit(struct trapframe *trapframe);
int sys_mbox_call(struct trapframe *trapframe);
int sys_kill(struct trapframe *trapframe);
int sys_signal(struct trapframe *trapframe);
int sys_sigkill(struct trapframe *trapframe);
int sys_sigreturn(struct trapframe *trapframe);
int sys_open(struct trapframe *trapframe);
int sys_close(struct trapframe *trapframe);
int sys_write(struct trapframe *trapframe);
int sys_read(struct trapframe *trapframe);
int sys_mkdir(struct trapframe *trapframe);
int sys_mount(struct trapframe *trapframe);
int sys_chdir(struct trapframe *trapframe);

typedef int (*syscall_t)(struct trapframe *);
extern syscall_t sys_call_table[SYSCALL_NUM];

#endif // __ASSEMBLER__
#endif // __SYSCALL_H__