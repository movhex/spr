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

#if !defined(SPR_INCLUDE_INTERNAL_HEADERS)
#if defined(_MSC_VER)
#pragma message("spr_portable_freebsd.h is an internal header file and must \
not be used directly. Please use spr_portable.h instead.")
#else
#warning "spr_portable_freebsd.h is an internal header file and must not be \
used directly. Please use spr_portable.h instead."
#endif
#define SPR_INCLUDE_INTERNAL_HEADERS
#endif

#ifndef INCLUDED_SPR_PORTABLE_FREEBSD_H
#define INCLUDED_SPR_PORTABLE_FREEBSD_H

#ifdef __cplusplus
}
#endif

#if !defined(_BSD_SOURCE)
#define _BSD_SOURCE
#endif

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <glob.h>
#include <pthread.h>
#include <pthread_np.h>
#include <semaphore.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <pwd.h>
#include <grp.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>

#if (SPR_HAVE_MMAP)
#include <sys/mman.h>
#endif

#if (SPR_HAVE_GCD_SEM)
#include <dispatch/dispatch.h>
#endif

typedef int                            spr_fd_t;
typedef struct stat                    spr_file_stat_t;
typedef int                            spr_socket_t;
typedef socklen_t                      spr_socklen_t;
typedef uint32_t                       spr_tid_t;
typedef pthread_t                      spr_thread_handle_t;
typedef void *                         spr_thread_value_t;
typedef struct tm                      spr_tm_t;

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_SPR_PORTABLE_FREEBSD_H */
