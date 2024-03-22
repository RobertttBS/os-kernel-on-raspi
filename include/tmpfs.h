/* Reference the `ramfs` of linux. */
#ifndef __TMPFS_H__
#define __TMPFS_H__


int tmpfs_mount(struct file_system_type* fs, struct mount* mount);

struct file_system_type tmpfs_fs_type = {
    .name = "tmpfs",
    .mount = tmpfs_mount,
};

/* inode->internal will be the pointer of this structure */
struct tmpfs_inode {
    struct inode *inode; // point back to the inode.
};

int tmpfs_lookup(struct inode* dir_node, struct inode** target, const char* component_name);
int tmpfs_create(struct inode* dir_node, struct inode** target, const char* component_name);

struct inode_operations tmpfs_dir_inode_operations = {
    .lookup = tmpfs_lookup,
    .create = tmpfs_create,
};

int tmpfs_write(struct file* file, const void* buf, size_t len);
int tmpfs_read(struct file* file, void* buf, size_t len);

struct file_operations tmpfs_file_operations = {
    .write = tmpfs_write,
    .read = tmpfs_read,
};

#endif // __TMPFS_H__