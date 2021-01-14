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

#ifndef INCLUDED_SPR_ERRNO_H
#define INCLUDED_SPR_ERRNO_H

#include "spr_portable.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPR_ERRNO_STR_SIZE           1024

#if (SPR_POSIX)
#define spr_get_errno()              (errno)
#define spr_set_errno(err)           (errno = err)
#define spr_get_socket_errno()       (errno)
#define spr_set_socket_errno()       (errno = err)

#elif (SPR_WIN32)
#define spr_get_errno()              GetLastError()
#define spr_set_errno(err)           SetLastError(err)
#define spr_get_socket_errno()       WSAGetLastError()
#define spr_set_socket_errno(err)    WSASetLastError(err)
#endif

#define SPR_OK                       0
#define SPR_FAILED                  -1
#define SPR_BUSY                    -2
#define SPR_DONE                    -3
#define SPR_ABORT                   -4
#define SPR_DECLINED                -5
#define SPR_NOT_FOUND               -6

/* Where the SPR specific error values start */
#define SPR_ERROR_DOMAIN_BASE        20000
#define SPR_ERRSPACE_SIZE            100
#define SPR_ERROR_DOMAIN(domain) \
    (SPR_ERROR_DOMAIN_BASE + (domain * SPR_ERRSPACE_SIZE))

#define SPR_ERR_FILESYS              SPR_ERROR_DOMAIN(1)
#define SPR_ERR_FILESYS_ABS_PATH     (SPR_ERR_FILESYS+1)
#define SPR_ERR_FILESYS_LONG_PATH    (SPR_ERR_FILESYS+2)


const char *spr_strerror(spr_err_t err, char *buf, size_t bufsize);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_SPR_ERRNO_H */
