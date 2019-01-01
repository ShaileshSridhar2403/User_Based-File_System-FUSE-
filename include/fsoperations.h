#ifndef OPERATIONS
#define OPERATIONS

#include<bits/stdc++.h>
#include <fuse.h>
#include <unistd.h>

#include "fstree.h"

#define ll unsigned long long

using namespace std;

extern FSMD* root;

int fs_getattr(const char *path, struct stat *st);

int fs_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi );

int fs_read(const char *path, char *buf, size_t size, off_t offset,struct fuse_file_info *fi);

int fs_mkdir(const char *path,mode_t mode);

int fs_mknod(const char *path,mode_t mode,dev_t dev);

int fs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi);

int fs_open(const char *path, struct fuse_file_info *fi);

int fs_rmdir(const char * path);

int fs_unlink(const char *path);

int fs_statfs(const char* path, struct statvfs* stbuf);

int fs_rename (const char *old_name_path, const char *new_name_path);

int fs_access(const char * path, int mask);

int fs_chmod(const char *path, mode_t newp);

int fs_truncate(const char *path, off_t size);

#endif
