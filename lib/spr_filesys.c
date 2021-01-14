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

#include "spr_portable.h"
#include "spr_filesys.h"
#include "spr_errno.h"
#include "spr_memory.h"
#include "spr_string.h"


#if (SPR_POSIX)

static spr_file_type_t
spr_stat_get_file_type(spr_file_stat_t *stat)
{
    spr_file_type_t type;
    mode_t mode;

    type = SPR_FILE_TYPE_UNKFILE;
    mode = stat->st_mode;

    if (S_ISREG(mode)) {
        type = SPR_FILE_TYPE_FILE;
    }
    if (S_ISDIR(mode)) {
        type = SPR_FILE_TYPE_DIR;
    }
    if (S_ISCHR(mode)) {
        type = SPR_FILE_TYPE_CHR;
    }

#if defined(S_ISBLK)
    if (S_ISBLK(mode)) {
        type = SPR_FILE_TYPE_BLK;
    }
#endif

#if defined(S_ISFIFO)
    if (S_ISFIFO(mode)) {
        type = SPR_FILE_TYPE_PIPE;
    }
#endif

#if defined(S_ISLNK)
    if (S_ISLNK(mode)) {
        type = SPR_FILE_TYPE_LINK;
    }
#endif

#if defined(S_ISLNK)
    if (S_ISSOCK(mode)) {
        type = SPR_FILE_TYPE_SOCK;
    }
#endif

    return type;
}

static spr_file_access_t
spr_stat_get_access(spr_file_stat_t *stat)
{
    spr_file_access_t access;
    mode_t mode;

    access = 0;
    mode = stat->st_mode;

    if (mode & S_ISUID) {
        access |= SPR_FILE_ACCESS_USETID;
    }
    if (mode & S_IRUSR) {
        access |= SPR_FILE_ACCESS_UR;
    }
    if (mode & S_IWUSR) {
        access |= SPR_FILE_ACCESS_UW;
    }
    if (mode & S_IXUSR) {
        access |= SPR_FILE_ACCESS_UX;
    }

    if (mode & S_ISGID) {
        access |= SPR_FILE_ACCESS_GSETID;
    }
    if (mode & S_IRGRP) {
        access |= SPR_FILE_ACCESS_GR;
    }
    if (mode & S_IWGRP) {
        access |= SPR_FILE_ACCESS_GW;
    }
    if (mode & S_IXGRP) {
        access |= SPR_FILE_ACCESS_GX;
    }

#if defined(S_ISVTX)
    if (mode & S_ISVTX) {
        access |= SPR_FILE_ACCESS_STICKY;
    }
#endif

    if (mode & S_IROTH) {
        access |= SPR_FILE_ACCESS_OR;
    }
    if (mode & S_IWOTH) {
        access |= SPR_FILE_ACCESS_OW;
    }
    if (mode & S_IXOTH) {
        access |= SPR_FILE_ACCESS_OX;
    }

    return access;
}

spr_err_t
spr_file_open(spr_file_t *file, const char *path, spr_uint_t mode,
    spr_uint_t create, spr_uint_t access)
{
    spr_fd_t fd;

    spr_memzero(file, sizeof(spr_file_t));

    fd = open(path, mode|create, access);
    if (fd == SPR_INVALID_FILE) {
        return spr_get_errno();
    }

    file->fd = fd;
    file->name = (char *) path;
    file->offset = (size_t) 0;

    return SPR_OK;
}

ssize_t
spr_file_read(spr_file_t *file, uint8_t *buf, size_t size,
    spr_off_t offset)
{
    ssize_t n;

    if (lseek(file->fd, offset, SEEK_SET) == -1) {
        return -1;
    }

    n = read(file->fd, buf, size);
    if (n == -1) {
        return -1;
    }

    file->offset += n;
    return n;
}

ssize_t
spr_file_write(spr_file_t *file, const char *buf, size_t size,
    spr_off_t offset)
{
    ssize_t n;
    ssize_t written = 0;

    if (lseek(file->fd, offset, SEEK_SET) == -1) {
        return -1;
    }

    for ( ; ; ) {
        n = write(file->fd, buf + written, size);
        if (n == -1) {
            return -1;
        }
        file->offset += n;
        written += n;
        if ((size_t) n == size) {
            return written;
        }
        size -= n;
    }
}

/* Work even we already read some bytes from file */
ssize_t
spr_file_size(spr_file_t *file)
{
    spr_off_t off;
    size_t size;

    off = lseek(file->fd, 0, SEEK_CUR);
    if (off == (spr_off_t) -1) {
        return -1;
    }
    size = (size_t) lseek(file->fd, 0, SEEK_END);
    if (size == (size_t) -1) {
        return -1;
    }
    if (lseek(file->fd, off, SEEK_SET) == (spr_off_t) -1) {
        return -1;
    }
    return size;
}

ssize_t
spr_read_fd(spr_fd_t fd, void *buf, size_t n)
{
    ssize_t rv;

    rv = read(fd, buf, n);
    if (rv == -1) {
        return -1;
    }
    return rv;
}

ssize_t
spr_write_fd(spr_fd_t fd, void *buf, size_t n)
{
    ssize_t rv;

    rv = write(fd, buf, n);
    if (rv == -1) {
        return -1;
    }
    return rv;
}


spr_err_t
spr_file_info_by_path(spr_file_info_t *info, const char *path)
{
    spr_err_t err;

    if (stat(path, &(info->stat)) == -1) {
        err = spr_get_errno();
        return err;
    }
    info->type = spr_stat_get_file_type(&(info->stat));
    info->access = spr_stat_get_access(&(info->stat));

    return SPR_OK;
}

spr_err_t
spr_file_info(spr_file_info_t *info, spr_file_t *file)
{
    spr_err_t err;

    if (fstat(file->fd, &(info->stat)) == -1) {
        err = spr_get_errno();
        return err;
    }
    info->type = spr_stat_get_file_type(&(info->stat));
    info->access = spr_stat_get_access(&(info->stat));

    return SPR_OK;
}

void
spr_file_close(spr_file_t *file)
{
    if (!file) {
        return;
    }

    close(file->fd);
    file->fd = SPR_INVALID_FILE;
    file->name = NULL;
    file->offset = (spr_off_t) 0;
}

spr_err_t
spr_dir_open(spr_dir_t *dir, const char *path)
{
    spr_err_t err;

    spr_memzero(dir, sizeof(spr_dir_t));

    dir->dir = opendir(path);
    if (dir->dir == NULL) {
        err = spr_get_errno();
        return err;
    }

    dir->name = path;
    /*
     * Set by spr_memset()
     * dir-type = SPR_FILE_TYPE_UNKFILE;
     */

    return SPR_OK;
}

#if (SPR_HAVE_D_TYPE)

spr_err_t
spr_dir_read(spr_dir_t *dir)
{
    spr_err_t err;

    dir->dirent = readdir(dir->dir);
    if (!dir->dirent) {
        err = spr_get_errno();
        if (err != SPR_OK) {
            return err;
        }
        return SPR_DONE;
    }

    dir->type = dir->dirent->d_type;

    return SPR_OK;
}
#else

spr_err_t
spr_dir_read(spr_dir_t *dir)
{
    spr_file_stat_t fstat;
    char fullpath[SPR_MAX_PATH_LEN];
    spr_err_t err;

    dir->dirent = readdir(dir->dir);
    if (!dir->dirent) {
        err = spr_get_errno();
        if (err != SPR_OK) {
            return err;
        }
        return SPR_DONE;
    }

    spr_file_full_path(fullpath, dir->name, dir->dirent->d_name);

    spr_file_stat_by_path(&fstat, fullpath);

    dir->type = spr_stat_get_file_type(&fstat);

    return SPR_OK;
}
#endif


void
spr_dir_close(spr_dir_t *dir)
{
    if (!dir) {
        return;
    }
    closedir(dir->dir);
}

const char *
spr_dir_current_name(spr_dir_t *dir)
{
    return dir->dirent->d_name;
}

spr_err_t
spr_glob_open(spr_glob_t *gl, const char *pattern)
{
    int rv;

    rv = glob(pattern, 0, NULL, &gl->glob);
    if (rv != 0) {
        return spr_get_errno();
    }

    gl->pattern = pattern;
    gl->last = 0;

    return SPR_OK;
}

spr_err_t
spr_glob_read(spr_glob_t *gl, char *res)
{
    size_t n, len;

#ifdef GLOB_NOMATCH
    n = (size_t) gl->glob.gl_pathc;
#else
    n = (size_t) gl->glob.gl_matchc;
#endif

    if (gl->last >= n) {
        return SPR_DONE;
    }

    len = strlen(gl->glob.gl_pathv[gl->last]);
    memmove(res, gl->glob.gl_pathv[gl->last], len + 1);
    gl->last += 1;

    return SPR_OK;
}

void
spr_glob_close(spr_glob_t *gl)
{
    globfree(&gl->glob);
}


spr_err_t
spr_check_absolute_path(const char *path)
{
    if (*path == '/') {
        return SPR_OK;
    }

    return SPR_ERR_FILESYS_ABS_PATH;
}

#elif (SPR_WIN32)

static uint32_t
spr_utf8_decode(u_char **p, size_t n)
{
    size_t len;
    uint32_t u, i, valid;

    u = **p;

    if (u >= 0xf0) {
        u &= 0x07;
        valid = 0xffff;
        len = 3;
    }
    else if (u >= 0xe0) {
        u &= 0x0f;
        valid = 0x7ff;
        len = 2;
    }
    else if (u >= 0xc2) {
        u &= 0x1f;
        valid = 0x7f;
        len = 1;
    }
    else {
        (*p)++;
        return 0xffffffff;
    }

    if (n - 1 < len) {
        return 0xfffffffe;
    }

    (*p)++;

    while (len) {
        i = *(*p)++;

        if (i < 0x80) {
            return 0xffffffff;
        }

        u = (u << 6) | (i & 0x3f);

        len--;
    }

    if (u > valid) {
        return u;
    }

    return 0xffffffff;
}

static uint16_t *
spr_path_to_wchar(uint16_t *outstr, size_t outlen,
    const uint8_t *instr)
{
    uint8_t *p;
    uint16_t *u, *last;
    uint32_t n;

    p = (uint8_t *) instr;
    u = outstr;
    last = outstr + outlen;

    while (u < last) {

        if (*p < 0x80) {
            *u++ = (uint16_t) *p;

            if (*p == 0) {
                return outstr;
            }

            p++;

            continue;
        }

        if (u + 1 == last) {
            break;
        }

        n = spr_utf8_decode(&p, 4);

        if (n > 0x10ffff) {
            return NULL;
        }

        if (n > 0xffff) {
            n -= 0x10000;
            *u++ = (uint16_t) (0xd800 + (n >> 10));
            *u++ = (uint16_t) (0xdc00 + (n & 0x03ff));
            continue;
        }

        *u++ = (uint16_t) n;
    }
    return NULL;
}

static spr_file_type_t
spr_stat_get_file_type(spr_file_stat_t *stat)
{
    spr_file_type_t type;

    type = SPR_FILE_TYPE_UNKFILE;

    if (stat->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        type = SPR_FILE_TYPE_DIR;
    }
    if (!(stat->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
        type = SPR_FILE_TYPE_FILE;
    }

    return type;
}

spr_err_t
spr_file_open(spr_file_t *file, const char *path, spr_uint_t mode,
    spr_uint_t create, spr_uint_t access)
{
    uint16_t wpath[SPR_MAX_PATH_LEN];
    spr_fd_t fd;

    (void) access;

    spr_memzero(file, sizeof(spr_file_t));

    spr_path_to_wchar(wpath, sizeof(wpath), (const uint8_t *) path);

    fd = CreateFileW(wpath, mode, 0, NULL, create, 0, NULL);
    if (fd == SPR_INVALID_FILE) {
        return spr_get_errno();
    }

    file->fd = fd;
    file->name = (char *) path;
    file->offset = (size_t) 0;

    return SPR_OK;
}

ssize_t
spr_file_read(spr_file_t *file, uint8_t *buf, size_t size,
    spr_off_t offset)
{
    DWORD n;
    OVERLAPPED ovlp;

    ovlp.Internal = 0;
    ovlp.InternalHigh = 0;
    ovlp.Offset = (DWORD) offset;
    /* ovlp.OffsetHigh = (DWORD) (offset >> 32); */
    ovlp.hEvent = NULL;

    if (ReadFile(file->fd, buf, size, &n, &ovlp) == 0) {
        return -1;
    }
    file->offset += n;
    return (ssize_t) n;
}

ssize_t
spr_file_write(spr_file_t *file, const char *buf, size_t size,
    spr_off_t offset)
{
    DWORD n;
    OVERLAPPED ovlp;

    ovlp.Internal = 0;
    ovlp.InternalHigh = 0;
    ovlp.Offset = (DWORD) offset;
    /* ovlp.OffsetHigh = (DWORD) (offset >> 32); */
    ovlp.hEvent = NULL;

    if (WriteFile(file->fd, buf, size, &n, &ovlp) == 0) {
        return -1;
    }

    if (((size_t) n) != size) {
        return -1;
    }
    file->offset += n;
    return n;
}

#if (SPR_PTR_SIZE == 8)

ssize_t
spr_file_size(spr_file_t *file)
{
    LARGE_INTEGER info;
    ssize_t size;

    if (GetFileSizeEx(file->fd, &info) != 1) {
        return -1;
    }
    size = info.QuadPart;

    return (ssize_t) size;
}
#else

ssize_t
spr_file_size(spr_file_t *file)
{
    ssize_t size;

    size = (ssize_t) GetFileSize(file->fd, NULL);
    if (size == SPR_INVALID_FILE_SIZE) {
        return -1;
    }
    return (ssize_t) size;
}
#endif

ssize_t
spr_read_fd(spr_fd_t fd, void *buf, size_t size)
{
    DWORD n;

    if (ReadFile(fd, buf, size, &n, NULL) == 0) {
        return -1;
    }
    return (ssize_t) n;
}

ssize_t
spr_write_fd(spr_fd_t fd, void *buf, size_t size)
{
    DWORD n;

    if (WriteFile(fd, buf, size, &n, NULL) == 0) {
        return -1;
    }
    return (ssize_t) n;
}

spr_err_t
spr_file_stat_by_path(spr_file_stat_t *stat, const char *path)
{
    WIN32_FILE_ATTRIBUTE_DATA file_attr;
    uint16_t wpath[SPR_MAX_PATH_LEN];

    spr_path_to_wchar(wpath, sizeof(wpath), (const uint8_t *) path);

    if (GetFileAttributesExW(wpath, GetFileExInfoStandard, &file_attr) == 0) {
        return spr_get_errno();
    }

    stat->dwFileAttributes = file_attr.dwFileAttributes;
    stat->ftCreationTime = file_attr.ftCreationTime;
    stat->ftLastAccessTime = file_attr.ftLastAccessTime;
    stat->ftLastWriteTime = file_attr.ftLastWriteTime;
    stat->nFileSizeHigh = file_attr.nFileSizeHigh;
    stat->nFileSizeLow = file_attr.nFileSizeLow;

    return SPR_OK;
}

spr_err_t
spr_file_info_by_path(spr_file_info_t *info, const char *path)
{
    spr_err_t err;

    err = spr_file_stat_by_path(&(info->stat), path);
    if (err != SPR_OK) {
        return err;
    }

    info->type = spr_stat_get_file_type(&(info->stat));
    info->access = 0;

    return SPR_OK;
}

spr_err_t
spr_file_info(spr_file_info_t *info, spr_file_t *file)
{
    if (GetFileInformationByHandle(file->fd, &(info->stat)) == -1) {
        return spr_get_errno();
    }

    info->type = spr_stat_get_file_type(&(info->stat));
    info->access = 0;

    return SPR_OK;
}

void
spr_file_close(spr_file_t *file)
{
    if (!file) {
        return;
    }

    CloseHandle(file->fd);
    file->fd = SPR_INVALID_FILE;
    file->name = NULL;
    file->offset = (spr_off_t) 0;
}

spr_err_t
spr_dir_open(spr_dir_t *dir, const char *path)
{
    char str[SPR_MAX_PATH_LEN + 3];
    size_t len;
    spr_err_t err;

    spr_memzero(dir, sizeof(spr_dir_t));

    len = spr_strlen(path);

    if (len >= SPR_MAX_PATH_LEN + 3) {
        return E2BIG;
    }

    spr_memmove(str, path, len);
    str[len + 0] = '\\';
    str[len + 1] = '*';
    str[len + 2] = '\0';

    dir->dir = FindFirstFile((const char *) str, &(dir->finddata));
    if (dir->dir == INVALID_HANDLE_VALUE) {
        err = spr_get_errno();
        if (err != SPR_OK) {
            return err;
        }
        return SPR_DONE;
    }

    dir->name = path;
    /*
     * Set by spr_memset()
     * dir-type = SPR_FILE_TYPE_UNKFILE;
     */

    return SPR_OK;
}

spr_err_t
spr_dir_read(spr_dir_t *dir)
{
    spr_file_stat_t fstat;
    char fullpath[SPR_MAX_PATH_LEN];
    spr_err_t err;

    if (FindNextFile(dir->dir, &dir->finddata) == 0) {
        err = spr_get_errno();
        if (err != SPR_OK && err != ERROR_NO_MORE_FILES) {
            return err;
        }
        return SPR_DONE;
    }

    spr_file_full_path(fullpath, dir->name, dir->finddata.cFileName);

    spr_file_stat_by_path(&fstat, fullpath);

    dir->type = spr_stat_get_file_type(&fstat);

    return SPR_OK;
}

void
spr_dir_close(spr_dir_t *dir)
{
    if (!dir) {
        return;
    }
    FindClose(dir->dir);
}

spr_err_t
spr_glob_open(spr_glob_t *gl, const char *pattern)
{
    spr_int_t i;
    size_t len;

    gl->dir = FindFirstFile((const char *) pattern, &gl->finddata);
    if (gl->dir == INVALID_HANDLE_VALUE) {
        return SPR_FAILED;
    }

    len = 0;

    for (i = spr_strlen(pattern); i >= 0; --i) {
        if (pattern[i] == '\\') {
            len = (size_t) i;
            break;
        }
    }

    if (len == 0) {
        return SPR_ERR_FILESYS_ABS_PATH;
    }

    gl->prefix_len = len + 1;
    gl->last_res = gl->finddata.cFileName;
    gl->pattern = pattern;
    gl->done = 0;

    return SPR_OK;
}

spr_err_t
spr_glob_read(spr_glob_t *gl, char *res)
{
    size_t len, temp;
    spr_err_t err;

    if (gl->done) {
        return SPR_DONE;
    }

    temp = spr_strlen(gl->last_res);

    len = gl->prefix_len + temp + 1;
    if (SPR_MAX_PATH_LEN < len) {
        return SPR_ERR_FILESYS_LONG_PATH;
    }

    spr_memmove(res, gl->pattern, gl->prefix_len);
    spr_memmove(res + gl->prefix_len, gl->last_res, temp + 1);

    if (FindNextFile(gl->dir, &gl->finddata) != 0) {

        err = spr_get_errno();
        if (err != SPR_OK && err != ERROR_NO_MORE_FILES) {
            gl->done = 1;
            return err;
        }
        gl->last_res = gl->finddata.cFileName;
    }
    else {
        gl->done = 1;
    }

    return SPR_OK;
}

void
spr_glob_close(spr_glob_t *gl)
{
    FindClose(gl->dir);
}

const char *
spr_dir_current_name(spr_dir_t *dir)
{
    return dir->finddata.cFileName;
}

spr_err_t
spr_check_absolute_path(const char *path)
{
    /*
     * C:\path\to\foo.txt - absolute
     * C:path\to\foo.txt  - non-absolute
     * 
     * UNC-paths
     * \\server1\share\test\foo.txt
     * 
     */

    if (spr_isalpha(path[0]) && path[1] == ':' && path[2] == '\\') {
        return SPR_OK;
    }
    else if (path[0] == '\\' && path[1] == '\\') {
        return SPR_OK;
    }
    else {
        return SPR_ERR_FILESYS_ABS_PATH;
    }
}

#endif

ssize_t
spr_write_stdout(const char *str)
{
    return spr_write_fd(SPR_STDOUT, (void *) str, strlen(str));
}

ssize_t
spr_write_stderr(const char *str)
{
    return spr_write_fd(SPR_STDERR, (void *) str, strlen(str));
}

spr_file_type_t
spr_dir_current_file_type(spr_dir_t *dir)
{
    return dir->type;
}

spr_err_t
spr_dir_current_is_file(spr_dir_t *dir)
{
    return (dir->type == SPR_FILE_TYPE_FILE);
}

size_t
spr_file_full_path(char *buf, const char *path, const char *file)
{
    size_t path_len, file_len, copied;

    path_len = spr_strlen(path);
    file_len = spr_strlen(file);

    if (spr_is_path_separator(path[path_len - 1])) {
        path_len -= 1;
    }

    copied = 0;

    spr_memmove(buf + copied, path, path_len);
    copied += path_len;

    spr_memmove(buf + copied, SPR_PATH_SEPARATOR_STR, 1);
    copied += + 1;

    spr_memmove(buf + copied, file, file_len);
    copied += file_len;

    buf[copied] = '\0';

    return copied;
}

bool
spr_is_path_has_wildcard(const char *path)
{
    char *ch;

    ch = (char *) path;

    while (*ch) {
        switch (*ch) {
        case '?':
        case '*':
            return 1;
        }
        ++ch;
    }
    return 0;
}
