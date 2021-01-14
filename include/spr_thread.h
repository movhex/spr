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

#ifndef INCLUDED_SPR_THREAD_H
#define INCLUDED_SPR_THREAD_H

#include "spr_portable.h"
#include "spr_bitfield.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Thread specific parameters */
#define SPR_THREAD_SCOPE_SYSTEM           0x00000001
#define SPR_THREAD_SCOPE_PROCESS          0x00000002
#define SPR_THREAD_CREATE_DETACHED        0x00000004
#define SPR_THREAD_CREATE_JOINABLE        0x00000008

#if (SPR_POSIX)

/* TODO: Add priority for unix threads */
#define SPR_THREAD_PRIORITY_HIGHEST       0
#define SPR_THREAD_PRIORITY_ABOVE_NORMAL  0
#define SPR_THREAD_PRIORITY_NORMAL        0
#define SPR_THREAD_PRIORITY_BELOW_NORMAL  0
#define SPR_THREAD_PRIORITY_LOWEST        0

#define spr_thread_current_handle()       pthread_self()
#define spr_thread_equal(t1, t2)          pthread_equal(t1, t2)
#define spr_thread_exit(retval)  pthread_exit((spr_thread_value_t) retval)

#if (SPR_LINUX)
#define SPR_FMT_TID_T                     "%d"
#elif (SPR_FREEBSD)
#define SPR_FMT_TID_T                     "%u"
#elif (SPR_DARWIN)
#define SPR_FMT_TID_T                     "%lu"
#else
#define SPR_FMT_TID_T                     "%lu"
#endif

#elif (SPR_WIN32)

#define SPR_THREAD_PRIORITY_HIGHEST       THREAD_PRIORITY_HIGHEST
#define SPR_THREAD_PRIORITY_ABOVE_NORMAL  THREAD_PRIORITY_ABOVE_NORMAL
#define SPR_THREAD_PRIORITY_NORMAL        THREAD_PRIORITY_NORMAL
#define SPR_THREAD_PRIORITY_BELOW_NORMAL  THREAD_PRIORITY_BELOW_NORMAL
#define SPR_THREAD_PRIORITY_LOWEST        THREAD_PRIORITY_LOWEST

#define spr_thread_current_handle()       GetCurrentThread()
#define spr_thread_equal(t1, t2)          (t1 == t2)
#define spr_thread_exit(retval)  ExitThread((spr_thread_value_t) retval)

#define SPR_FMT_TID_T                     "%lu"

#endif

typedef spr_thread_value_t (*spr_thread_function_t)(void *);

struct spr_thread_s {
    spr_thread_handle_t handle;
};


spr_err_t spr_thread_init(spr_thread_t *thread, spr_bitfield_t params,
    size_t stack_size, spr_int_t prio, spr_thread_function_t start_routine,
    void *arg);
spr_err_t spr_thread_join(spr_thread_t *thread);
spr_err_t spr_thread_cancel(spr_thread_t *thread);
void spr_thread_fini(spr_thread_t *thread);

spr_tid_t spr_thread_current_tid(void);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_SPR_THREAD_H */
