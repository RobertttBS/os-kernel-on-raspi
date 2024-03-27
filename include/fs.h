#ifndef __FS_H__
#define __FS_H__

#include "stdlib.h"
#include "list.h"
#include "stdint.h"


/* Ref: include/linux/dcache.h */
#define DCACHE_ENTRY_TYPE       0x00700000
#define DCACHE_REGULAR_TYPE     0x00100000    /* Regular file type */
#define DCACHE_DIRECTORY_TYPE   0x00200000    /* Normal directory */

#define FILE_NAME_LEN    32

#define O_CREAT 0x40  /* lab 7 spec defined value. */

struct path {
    struct vfsmount *mnt;
    struct dentry *dentry;
};

struct inode {
    struct super_block *i_sb;

    struct inode_operations *i_op;
    struct file_operations *i_fop;

    void *internal;
};

struct file {
    struct inode *f_inode;
    struct file_operations *f_op;

    struct path f_path;

    size_t f_pos; /* The next read/write position of this file descriptor. */
    int f_flags; /* Not sure the usage for now. */
};

struct super_block {
    struct file_system_type *s_type;
    struct super_operations *s_op;

    struct dentry *s_root;
    struct dentry_operations *s_d_op; /* default d_op for dentries*/

    struct list_head s_inodes; /* All inodes. */
};

struct dentry {
    unsigned int d_flags; /* Represent the dentry type*/
    struct inode *d_inode;
    struct dentry *d_parent;
    char d_iname[FILE_NAME_LEN];

    struct dentry_operations *d_op;
    struct super_block *d_sb;

    // struct list_head d_lru; /* it is for lru cache usage, to cache the useless inode structure. No need for now */
    struct list_head d_child; /* parent's child, so it is sibling dentry. */
    struct list_head d_subdirs; /* sub directory of this dentry (children dentry) */
};

struct mount {
    struct dentry *mnt_root;
    struct super_block *mnt_sb;
};

/**
 * In linux kernel, `mount` is a function api to generate `struct dentry` for vfsmount.
 * Here, the `mount` function should generate a `struct dentry` with the given `dir_name`.
*/
struct file_system_type {
    const char *name;
    struct dentry *(*mount) (struct file_system_type *fs, const char *dir_name);
    struct file_system_type *next;
};

/* ref: `simple_dir_operations` */
struct file_operations {
    /* ref: `simple_open`: move `inode->i_private` to `file->private_data` */
    int (*write) (struct file *file, const void *buf, size_t len);
    int (*read) (struct file *file, void *buf, size_t len); /* ref: `generic_read_dir` */
    /* ref: `generic_file_llseek`, `default_llseek` */
};

struct inode_operations {
    int (*lookup) (struct inode *dir_node, struct inode **target, const char *component_name); /* ref: `simple_lookup` */
    int (*create) (struct inode *dir_node, struct inode **target, const char *component_name); /* ref: `exfat_create`, `ex2_create`. Create inode for dentry (dir_node) */
    int (*mkdir) (struct inode *dir_node, struct dentry *dentry, int mode);
};

struct dentry_operations {
    int (*d_compare) (const struct dentry *dentry, const char *name); /* Not sure the implementation for now. */
};

struct super_operations {
    struct inode *(*alloc_inode) (struct super_block *sb);
};

extern struct mount *rootfs;
extern struct file_system_type *file_systems;


static inline bool d_is_directory(struct dentry *dentry)
{
    return (dentry->d_flags & DCACHE_ENTRY_TYPE) == DCACHE_DIRECTORY_TYPE;
}


int register_filesystem(struct file_system_type *fs); /* ref: register_filesystem` */
void vfs_init(void);

struct inode *new_inode(struct super_block *sb); /* fs provides this api to create a generic inode. */

struct file * vfs_open(const char *pathname, int flags);
int vfs_close(struct file *file);
int vfs_write(struct file *file, const void *buf, size_t len);
int vfs_read(struct file *file, void *buf, size_t len);

int vfs_mkdir(const char* pathname); /* ref: `vfs_mkdir` */
int vfs_mount(const char* target, const char* filesystem);
int vfs_lookup(const char* pathname, struct inode** target);
int vfs_create(const char* pathname); /* ref: `vfs_create`*/

#endif // __FS_H__