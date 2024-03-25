/* Reference the `ramfs` of linux. */
#ifndef __TMPFS_H__
#define __TMPFS_H__

/* inode->internal will be the pointer of this structure */
struct tmpfs_inode {
    struct inode *inode; // point back to the inode.
};

extern struct file_system_type tmpfs_fs_type;

/* Operations. */
extern struct inode_operations tmpfs_dir_inode_operations;
extern struct file_operations tmpfs_file_operations;
extern struct dentry_operations tmpfs_dentry_operations;
extern struct super_operations tmpfs_super_operations;

/* function in file_system_type. */
struct dentry *tmpfs_mount(struct file_system_type* fs, const char* dir_name);

/* Functions in tmpfs_inode_operations. */
int tmpfs_lookup(struct inode* dir_node, struct inode** target, const char* component_name);
int tmpfs_create(struct inode* dir_node, struct inode** target, const char* component_name);

/* Function in tmpfs_file_operations. */
int tmpfs_write(struct file* file, const void* buf, size_t len);
int tmpfs_read(struct file* file, void* buf, size_t len);

/* Function in tmpfs_dentry_operations. */
int tmpfs_d_compare(const struct dentry* dentry, const char* name);

#endif // __TMPFS_H__