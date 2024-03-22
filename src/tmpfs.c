#include "fs.h"
#include "tmpfs.h"

int tmpfs_mount(struct file_system_type *fs, struct mount *mount)
{
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
