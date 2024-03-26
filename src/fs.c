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

/* Ref: `dentry_open()` */
struct file* vfs_open(const char *pathname, int flags)
{
    struct inode *target;
    char target_path[64];
    struct file *file;

    // 1. Lookup pathname from the root inode.
    if (vfs_lookup(pathname, &target) != 0) {
        printf("File not found.\n");
        return NULL;
    }
    // 2. Create a new file descriptor for this inode if found.
    struct inode *target_file;
    if (rootfs->mnt_root->d_inode->i_op->lookup(target, target_path, &target_file) != 0) { // What's the purpose of inode lookup?
        printf("File not found.\n");
        return NULL;
    } else { // 3. Create a new file if O_CREAT is specified in flags.
        if (flags & O_CREAT) {
            file = (struct file *) kmalloc(sizeof(struct file));
            if (rootfs->mnt_root->d_inode->i_op->create(target, target_path, &target_file) != 0) {
                printf("Failed to create file.\n");
                return NULL;
            }
            
            file->f_inode = target_file;
            file->f_flags = flags;
            return file;
        }
    }

    return NULL;
}

int vfs_close(struct file *file)
{
    // 1. release the file descriptor
    kfree(file);
    return 0;
}

int vfs_write(struct file *file, const void *buf, size_t len)
{
    if (!(file->f_path.dentry->d_flags & DCACHE_REGULAR_TYPE)) {
        printf("Can't write to a non regular file.\n");
        return -1;
    }
    // 1. write len byte from buf to the opened file.
    // 2. return written size or error code if an error occurs.

    return file->f_op->write(file, buf, len);;
}

int vfs_read(struct file *file, void *buf, size_t len)
{
    if (!(file->f_path.dentry->d_flags & DCACHE_REGULAR_TYPE)) {
        printf("Can't write to a non regular file.\n");
        return -1;
    }
    // 1. read min(len, readable file data size) byte to buf from the opened file.
    // 2. return read size or error code if an error occurs.

    return file->f_op->read(file, buf, len);
}

int vfs_mkdir(const char *pathname)
{
    struct inode *target_dir, *child_dir;
    char child_name[64];

    vfs_lookup(pathname, &target_dir);
    if (target_dir->i_op->mkdir(target_dir, child_name, &child_dir) != 0) {
        printf("Failed to create directory.\n");
        return -1;
    }
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
 * Because my design, I think the target should be a dentry.
 */
int vfs_lookup(const char *pathname, struct inode **target)
{
    struct dentry *root_dentry = rootfs->mnt_root;
    struct dentry *p;
    char tmp[32], *tmp_ptr = tmp;
    
    /* Walk the first "/". */
    while (*pathname == '/')
        pathname++;
    if (!*pathname) {
        *target = root_dentry->d_inode;
        return 0;
    }

    while (*pathname != '\0') {
        /* Walk the path name from the children of parent dentry. */
        list_for_each_entry(p, &root_dentry->d_subdirs, d_subdirs) {
            while (*pathname != '/' && *pathname != '\0') {
                *tmp_ptr = *pathname;
                tmp_ptr++;
                pathname++;
            }
            if (!strcmp(p->d_iname, tmp)) {
                if (*pathname == '\0') {
                    *target = p->d_inode;
                    return 0;
                }
                /* Go to next level. */
                root_dentry = p;
                continue;
            }

            /* Reset for next round. */
            tmp_ptr = tmp;
            while (*pathname == '/')
                pathname++;
        }
    }

    *target = NULL;
    return -1;
}

/**
 * VFS provided function to allocate a generic inode.
*/
struct inode *new_inode(struct super_block *sb)
{
    struct inode *inode = (struct inode *) kmalloc(sizeof(struct inode));
    
    inode->i_sb = sb;
    return inode;
}