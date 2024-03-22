#include "fs.h"
#include "stddef.h"


struct mount* rootfs;


int register_file_system_type(struct file_system_type* fs)
{
    // register the file system to the kernel.
    // you can also initialize memory pool of the file system here.


    return 0;
}

struct file* vfs_open(const char* pathname, int flags)
{
    // 1. Lookup pathname from the root inode.
    // 2. Create a new file descriptor for this inode if found.
    // 3. Create a new file if O_CREAT is specified in flags.

    return NULL;
}

int vfs_close(struct file* file)
{
    // 1. release the file descriptor

    return 0;
}

int vfs_write(struct file* file, const void* buf, size_t len)
{
    // 1. write len byte from buf to the opened file.
    // 2. return written size or error code if an error occurs.

    return 0;
}

int vfs_read(struct file* file, void* buf, size_t len)
{
    // 1. read min(len, readable file data size) byte to buf from the opened file.
    // 2. return read size or error code if an error occurs.

    return 0;
}