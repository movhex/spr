/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 movhex <movhex@gmail.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef INCLUDED_SPR_FILESYS_H
#define INCLUDED_SPR_FILESYS_H

#include "spr_portable.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (SPR_POSIX)

/* Mode */
#define SPR_FILE_RDONLY                   O_RDONLY
#define SPR_FILE_WRONLY                   O_WRONLY
#define SPR_FILE_RDWR                     O_RDWR

/* Create */
#define SPR_FILE_OPEN                     0 /* Don't open if already existing */
#define SPR_FILE_CREATE_OR_OPEN           O_CREAT
#define SPR_FILE_TRUNCATE                 (O_CREAT|O_TRUNC)
#define SPR_FILE_APPEND                   (O_WRONLY|O_APPEND)
#define SPR_FILE_CREATE_OR_APPEND         (O_CREAT|O_APPEND|O_WRONLY)
#define SPR_FILE_NONBLOCK                 O_NONBLOCK

/* Access */
#define SPR_FILE_DEFAULT_ACCESS           0644
#define SPR_FILE_OWNER_ACCESS             0600

#define SPR_INVALID_FILE                 -1
#define SPR_FILE_ERROR                   -1

#define SPR_STDIN                         STDIN_FILENO
#define SPR_STDOUT                        STDOUT_FILENO
#define SPR_STDERR                        STDERR_FILENO

#define SPR_PATH_SEPARATOR_STR            "/"
#define SPR_PATH_SEPARATOR                '/'

#if defined(PATH_MAX)
#define SPR_MAX_PATH_LEN                  PATH_MAX
#else
/* #define SPR_MAX_PATH_LEN                  pathconf(path, _PC_PATH_MAX) */
#define SPR_MAX_PATH_LEN                  4096
#endif

#define SPR_FILE_ACCESS_USETID            0x8000
#define SPR_FILE_ACCESS_UR                0x0400
#define SPR_FILE_ACCESS_UW                0x0200
#define SPR_FILE_ACCESS_UX                0x0100

#define SPR_FILE_ACCESS_GSETID            0x4000
#define SPR_FILE_ACCESS_GR                0x0040
#define SPR_FILE_ACCESS_GW                0x0020
#define SPR_FILE_ACCESS_GX                0x0010

#define SPR_FILE_ACCESS_STICKY            0x2000
#define SPR_FILE_ACCESS_OR                0x0004
#define SPR_FILE_ACCESS_OW                0x0002
#define SPR_FILE_ACCESS_OX                0x0001

#if (SPR_HAVE_D_TYPE)
#define SPR_FILE_TYPE_UNKFILE  DT_UNKNOWN /* This is an unknown type */
#define SPR_FILE_TYPE_FILE     DT_REG     /* This is a regular file */
#define SPR_FILE_TYPE_DIR      DT_DIR     /* This is a directory */
#define SPR_FILE_TYPE_CHR      DT_CHR     /* This is a character device */
#define SPR_FILE_TYPE_BLK      DT_BLK     /* This is a block device */
#define SPR_FILE_TYPE_PIPE     DT_FIFO    /* This is a named pipe (FIFO) */
#define SPR_FILE_TYPE_LINK     DT_LNK     /* This is a symbolic link */
#define SPR_FILE_TYPE_SOCK     DT_SOCK    /* This is a UNIX domain socket */
#else
#define SPR_FILE_TYPE_UNKFILE  0          /* This is an unknown type */
#define SPR_FILE_TYPE_FILE     1          /* This is a regular file */
#define SPR_FILE_TYPE_DIR      2          /* This is a directory */
#define SPR_FILE_TYPE_CHR      3          /* This is a character device */
#define SPR_FILE_TYPE_BLK      4          /* This is a block device */
#define SPR_FILE_TYPE_PIPE     5          /* This is a named pipe (FIFO) */
#define SPR_FILE_TYPE_LINK     6          /* This is a symbolic link */
#define SPR_FILE_TYPE_SOCK     7          /* This is a UNIX domain socket */
#endif

struct spr_dir_s {
    DIR *dir;
    struct dirent *dirent;
    const char *name;
    uint8_t type;
};

struct spr_glob_s {
    glob_t glob;
    const char *pattern;
    size_t last;
};


#define spr_file_change_access(name, access) \
    chmod((const char *) name, access)

#define spr_file_rename(from, to) \
    rename((const char *) from, (const char *) to)

#define spr_file_delete(name)             unlink((const char *) name)
#define spr_dir_create(name, access)      mkdir((const char *) name, access)
#define spr_dir_delete(name)              rmdir((const char *) name)

#define spr_file_stat_by_path(st, path)   stat((const char *) path, st)
#define spr_file_stat_by_fd(st, fd)       fstat(fd, st)
#define spr_file_stat_by_link(st, path)   lstat((const char *) path, st)

#define spr_fsync(fd)                     fsync(fd)

#define spr_is_path_separator(c)          ((c) == '/')


#elif (SPR_WIN32)

/* Mode */
#define SPR_FILE_RDONLY                   GENERIC_READ
#define SPR_FILE_WRONLY                   GENERIC_WRITE
#define SPR_FILE_RDWR                     GENERIC_READ|GENERIC_WRITE

/* Create */
#define SPR_FILE_CREATE_OR_OPEN           OPEN_ALWAYS
#define SPR_FILE_OPEN                     OPEN_EXISTING
#define SPR_FILE_TRUNCATE                 CREATE_ALWAYS
#define SPR_FILE_APPEND                   FILE_APPEND_DATA|SYNCHRONIZE
#define SPR_FILE_CREATE_OR_APPEND         OPEN_ALWAYS /* fixme */
#define SPR_FILE_NONBLOCK                 0

/* Access */
#define SPR_FILE_DEFAULT_ACCESS           0
#define SPR_FILE_OWNER_ACCESS             0

#define SPR_INVALID_FILE                  INVALID_HANDLE_VALUE
#define SPR_FILE_ERROR                    0

#define SPR_STDIN                         GetStdHandle(STD_INPUT_HANDLE)
#define SPR_STDOUT                        GetStdHandle(STD_OUTPUT_HANDLE)
#define SPR_STDERR                        GetStdHandle(STD_ERROR_HANDLE)

#define SPR_PATH_SEPARATOR_STR            "\\"
#define SPR_PATH_SEPARATOR                '\\'

#define SPR_MAX_PATH_LEN                  MAX_PATH
#define SPR_INVALID_FILE_SIZE             INVALID_FILE_SIZE

#define SPR_FILE_TYPE_UNKFILE  0          /* This is an unknown type */
#define SPR_FILE_TYPE_FILE     1          /* This is a regular file */
#define SPR_FILE_TYPE_DIR      2          /* This is a directory */
#define SPR_FILE_TYPE_CHR      3          /* This is a character device */
#define SPR_FILE_TYPE_BLK      4          /* This is a block device */
#define SPR_FILE_TYPE_PIPE     5          /* This is a named pipe (FIFO) */
#define SPR_FILE_TYPE_LINK     6          /* This is a symbolic link */
#define SPR_FILE_TYPE_SOCK     7          /* This is a UNIX domain socket */

struct spr_dir_s {
    HANDLE dir;
    WIN32_FIND_DATA finddata;
    const char *name;
    uint8_t type;
};

struct spr_glob_s {
    HANDLE dir;
    WIN32_FIND_DATA finddata;
    const char *pattern;
    bool done;
    const char *last_res;
    size_t prefix_len;
};

#define spr_file_rename(from, to) \
    MoveFile((const char *) from, (const char *) to)

#define spr_file_delete(name)             DeleteFile((const char *) name)

#define spr_file_stat_by_fd(fd, fi)       GetFileInformationByHandle(fd, fi)
#define spr_file_stat_by_link(name, fi)   spr_file_stat_by_path(fi, name)

#define spr_fsync(fd)                     FlushFileBuffers(fd)

#define spr_is_path_separator(c)          ((c) == '\\')

spr_err_t spr_file_stat_by_path(spr_file_stat_t *stat, const char *path);

#endif

#define spr_dir_current_namelen(dir) \
    spr_strlen(spr_dir_current_name(dir))

struct spr_file_s {
    spr_fd_t fd;
    char *name;
    spr_off_t offset;
};

struct spr_file_info_s {
    spr_file_type_t type;
    spr_file_access_t access;
    spr_file_stat_t stat;
};


spr_err_t spr_file_open(spr_file_t *file, const char *path, spr_uint_t mode,
    spr_uint_t create, spr_uint_t access);
ssize_t spr_file_read(spr_file_t *file, uint8_t *buf, size_t size,
    spr_off_t offset);
ssize_t spr_file_write(spr_file_t *file, const char *buf, size_t size,
    spr_off_t offset);
ssize_t spr_file_size(spr_file_t *file);
void spr_file_close(spr_file_t *file);

ssize_t spr_read_fd(spr_fd_t fd, void *buf, size_t n);
ssize_t spr_write_fd(spr_fd_t fd, void *buf, size_t n);

ssize_t spr_write_stdout(const char *str);
ssize_t spr_write_stderr(const char *str);

spr_err_t spr_file_info_by_path(spr_file_info_t *info, const char *path);
spr_err_t spr_file_info(spr_file_info_t *info, spr_file_t *file);

spr_err_t spr_dir_open(spr_dir_t *dir, const char *path);
spr_err_t spr_dir_read(spr_dir_t *dir);
const char *spr_dir_name(spr_dir_t *dir);
const char *spr_dir_current_name(spr_dir_t *dir);
spr_file_type_t spr_dir_current_file_type(spr_dir_t *dir);
spr_err_t spr_dir_current_is_file(spr_dir_t *dir);
void spr_dir_close(spr_dir_t *dir);

int spr_glob_open(spr_glob_t *gl, const char *pattern);
int spr_glob_read(spr_glob_t *gl, char *res);
void spr_glob_close(spr_glob_t *gl);

size_t spr_file_full_path(char *buf, const char *path, const char *file);
bool spr_is_path_has_wildcard(const char *path);
spr_err_t spr_check_absolute_path(const char *path);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_SPR_FILESYS_H */
