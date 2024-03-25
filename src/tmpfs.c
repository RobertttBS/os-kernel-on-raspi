#include "fs.h"
#include "tmpfs.h"
#include "uart.h"
#include "string.h"
#include "mm.h"

struct inode *tmpfs_create_inode(struct super_block *sb);

struct file_system_type tmpfs_fs_type = {
    .name = "tmpfs",
    .mount = tmpfs_mount,
};

struct inode_operations tmpfs_inode_operations = {
    .lookup = tmpfs_lookup,
    .create = tmpfs_create,
};

struct file_operations tmpfs_file_operations = {
    .write = tmpfs_write,
    .read = tmpfs_read,
};

struct dentry_operations tmpfs_dentry_operations = {
    .d_compare = tmpfs_d_compare,
};

struct super_operations tmpfs_super_operations = {
    .alloc_inode = tmpfs_create_inode,
};

/* file system implement its own inode creation function. */
struct inode *tmpfs_create_inode(struct super_block *sb)
{
    struct inode *inode = get_inode(sb);
    
    inode->i_op = &tmpfs_inode_operations;
    inode->i_fop = &tmpfs_file_operations;
    /* TODO: Setup the internal, which is file system specific inode. */
    inode->internal = NULL;

    return inode;
}

/** 
 * Ref: __d_alloc()
 * TODO: I may need to implement a general dentry allocation function in fs.h.
 * So `tmpfs_create_dentry` can focus on the tmpfs specific dentry initialization.
 */
struct dentry *tmpfs_create_dentry(struct super_block *sb, const char *name, int type)
{
    struct dentry *dentry = (struct dentry *) kmalloc(sizeof(struct dentry));

    /* Initialize dentry members */
    dentry->d_flags |= type; /* Init the d_flags */
    dentry->d_inode = tmpfs_create_inode(sb); /* Init the d_inode */
    dentry->d_parent = dentry; /* Init the parent of the dentry */
    strcpy(dentry->d_iname, name); /* Init the name of the dentry */

    dentry->d_op = &tmpfs_dentry_operations; /* Init the d_op */
    dentry->d_sb = sb; /* Init the d_sb */

    INIT_LIST_HEAD(&dentry->d_child);
    INIT_LIST_HEAD(&dentry->d_subdirs);
    return dentry;
}

/**
 * Create a super block for the file system.
 * TODO: implement a general super_block creation function in fs.h.
 * Then `tmpfs_create_sb` can focus on the tmpfs specific super_block initialization.
*/
struct super_block *tmpfs_create_sb(struct file_system_type *fs)
{
    struct super_block *sb = (struct super_block *) kmalloc(sizeof(struct super_block));
    sb->s_type = fs;
    sb->s_op = &tmpfs_super_operations;
    sb->s_root = NULL;
    sb->s_d_op = &tmpfs_dentry_operations;
    INIT_LIST_HEAD(&sb->s_inodes);
    return sb;
}

/**
 * Mount the file system to the target path. VFS will call `file_system_type.mount()` and this is the underlay implementation.
 * 1. Create a super block and dentry for the file system.
 * 2. Return the dentry.
*/
struct dentry *tmpfs_mount(struct file_system_type *fs, const char *dir_name)
{
    struct dentry *dentry;
    struct super_block *sb;

    /* Allocate memory for superblock under the dentry and init the super_block.*/
    sb = tmpfs_create_sb(fs);
    /* Allocate memory for dentry and init it. */
    dentry = tmpfs_create_dentry(sb, dir_name, DCACHE_DIRECTORY_TYPE);

    sb->s_root = dentry;
    dentry->d_sb = sb;

    return dentry;
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

int tmpfs_d_compare(const struct dentry* dentry, const char* name)
{
    return 0;
}
