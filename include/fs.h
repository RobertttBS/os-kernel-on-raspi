#ifndef __FS_H__
#define __FS_H__

#include "stdlib.h"


struct inode {
  struct mount *mount;
  struct inode_operations *v_ops;
  struct file_operations *f_ops;
  void *internal;
};

struct file {
  struct inode *inode;
  size_t f_pos; // The next read/write position of this file descriptor
  struct file_operations *f_ops;
  int flags;
};

struct mount {
  struct inode *root;
  struct file_system_type *fs;
};

struct file_system_type {
  const char *name;
  int (*mount) (struct file_system_type *fs, struct mount *mount);
};

struct file_operations {
  int (*write) (struct file *file, const void *buf, size_t len);
  int (*read) (struct file *file, void *buf, size_t len);
};

struct inode_operations {
  int (*lookup) (struct inode *dir_node, struct inode **target, const char *component_name);
  int (*create) (struct inode *dir_node, struct inode **target, const char *component_name);
};

extern struct mount *rootfs;


int register_file_system_type(struct file_system_type *fs);

struct file * vfs_open(const char *pathname, int flags);
int vfs_close(struct file *file);
int vfs_write(struct file *file, const void *buf, size_t len);
int vfs_read(struct file *file, void *buf, size_t len);

#endif // __FS_H__