#include "fs.h"
#include "tmpfs.h"

struct dentry *tmpfs_mount(struct file_system_type *fs, const char *dir_name)
{
    /* Allocate memory for dentry and init it. */


    /* Allocate memory for superblock under the dentry and init the super_block.*/
    return 0;
}

int tmpfs_lookup(struct inode *dir_node, struct inode **target, const char *component_name)
{
    return 0;
}

int tmpfs_create(struct inode *dir_node, struct inode **target, const char *component_name)
{
    return 0;
}

int tmpfs_write(struct file *file, const void *buf, size_t len)
{
    return 0;
}

int tmpfs_read(struct file *file, void *buf, size_t len)
{
    return 0;
}
