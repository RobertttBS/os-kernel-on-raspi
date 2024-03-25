#include "fs.h"
#include "stddef.h"
#include "string.h"
#include "uart.h"
#include "mm.h"
#include "tmpfs.h"


struct mount* rootfs;
struct file_system_type *file_systems;

int register_filesystem(struct file_system_type *fs)
{
    // register the file system to the kernel.
    struct file_system_type **p;

    for (p = &file_systems; *p; p = &(*p)->next);
    *p = fs;

    return 0;
}

/* Initialize the tmpfs as root file system. */
void vfs_init()
{
    register_filesystem(&tmpfs_fs_type);
    vfs_mount("/", "tmpfs");
}

struct file* vfs_open(const char *pathname, int flags)
{
    // 1. Lookup pathname from the root inode.
    // 2. Create a new file descriptor for this inode if found.
    // 3. Create a new file if O_CREAT is specified in flags.

    return NULL;
}

int vfs_close(struct file *file)
{
    // 1. release the file descriptor

    return 0;
}

int vfs_write(struct file *file, const void *buf, size_t len)
{
    // 1. write len byte from buf to the opened file.
    // 2. return written size or error code if an error occurs.

    return 0;
}

int vfs_read(struct file *file, void *buf, size_t len)
{
    // 1. read min(len, readable file data size) byte to buf from the opened file.
    // 2. return read size or error code if an error occurs.

    return 0;
}

int vfs_mkdir(const char *pathname)
{
    return 0;
}


/**
 * Mount the file system to the target path.
 * Ref: linux `fc_mount()`.
*/
int vfs_mount(const char *target, const char *filesystem)
{
    struct file_system_type* fs;
    struct mount *mnt;

    mnt = (struct mount *) kmalloc(sizeof(struct mount));

    /* Find the registered file_system_type */
    for (fs = file_systems; fs; fs = fs->next)
        if (strcmp(fs->name, filesystem) == 0)
            break;
    if (!fs) {
        printf("File system not found.\n");
        return -1;
    }

    /* Use the file_system_type to generate `dentry` for given path. */
    mnt->mnt_root = fs->mount(fs, target);
    mnt->mnt_sb = mnt->mnt_root->d_sb;

    rootfs = mnt;
    printf("Success to mount %s to %s\n", filesystem, target);

    return 0;
}

/**
 * Find the inode correspond to path name, store it to target.
 */
int vfs_lookup(const char *pathname, struct inode **target)
{
    return 0;
}


/**
 * VFS provided function to allocate a general inode.
*/
struct inode *get_inode(struct super_block *sb)
{
    struct inode *inode = (struct inode *) kmalloc(sizeof(struct inode));
    
    inode->i_sb = sb;
    return inode;
}