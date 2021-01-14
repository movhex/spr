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

#ifndef INCLUDED_SPR_PORTABLE_H
#define INCLUDED_SPR_PORTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__linux__) || defined(__gnu_linux__)
#define SPR_SYSTEM_NAME  "GNU/Linux"
#define SPR_LINUX    1
#define SPR_FREEBSD  0
#define SPR_DARWIN   0
#define SPR_SOLARIS  0
#define SPR_POSIX    1
#define SPR_WIN32    0

#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#define SPR_SYSTEM_NAME  "FreeBSD"
#define SPR_LINUX    0
#define SPR_FREEBSD  1
#define SPR_DARWIN   0
#define SPR_SOLARIS  0
#define SPR_POSIX    1
#define SPR_WIN32    0

#elif defined(__APPLE__) && defined(__MACH__)
#define SPR_SYSTEM_NAME  "Darwin"
#define SPR_LINUX    0
#define SPR_FREEBSD  0
#define SPR_DARWIN   1
#define SPR_SOLARIS  0
#define SPR_POSIX    1
#define SPR_WIN32    0

#elif defined(__sun) && defined(__SVR4)
#define SPR_SYSTEM_NAME  "Solaris"
#define SPR_LINUX    0
#define SPR_FREEBSD  0
#define SPR_DARWIN   0
#define SPR_SOLARIS  1
#define SPR_POSIX    1
#define SPR_WIN32    0

#elif defined(_WIN32) || defined(__WIN32__)
#define SPR_SYSTEM_NAME  "Windows"
#define SPR_LINUX    0
#define SPR_FREEBSD  0
#define SPR_DARWIN   0
#define SPR_SOLARIS  0
#define SPR_POSIX    0
#define SPR_WIN32    1

#else
#error "Unsupported operating system"
#endif


#if (SPR_LINUX)
#include "spr_portable_linux.h"

#elif (SPR_FREEBSD)
#include "spr_portable_freebsd.h"

#elif (SPR_DARWIN)
#include "spr_portable_darwin.h"

#elif (SPR_SOLARIS)
#include "spr_portable_solaris.h"

#elif (SPR_WIN32)
#include "spr_portable_win32.h"

#endif

typedef int                            spr_err_t;
typedef intptr_t                       spr_int_t;
typedef uintptr_t                      spr_uint_t;
typedef size_t                         spr_size_t;
typedef ssize_t                        spr_ssize_t;
typedef intptr_t                       spr_off_t;
typedef uint32_t                       spr_bitfield_t;
typedef uintptr_t                      spr_msec_t;
typedef uintptr_t                      spr_file_access_t;
typedef uintptr_t                      spr_file_type_t;
typedef struct spr_file_s              spr_file_t;
typedef struct spr_dir_s               spr_dir_t;
typedef struct spr_glob_s              spr_glob_t;
typedef struct spr_file_info_s         spr_file_info_t;
typedef struct spr_thread_s            spr_thread_t;
typedef struct spr_mutex_s             spr_mutex_t;
typedef struct spr_semaphore_s         spr_semaphore_t;

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_SPR_PORTABLE_H */
