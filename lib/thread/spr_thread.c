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
#include "spr_thread.h"
#include "spr_errno.h"

#if (SPR_POSIX)

spr_err_t
spr_thread_init(spr_thread_t *thread, spr_bitfield_t params,
    size_t stack_size, spr_int_t prio, spr_thread_function_t start_routine,
    void *arg)
{
    pthread_attr_t attr;

    (void) prio;

    if (pthread_attr_init(&attr) != 0) {
        return spr_get_errno();
    }

    if (spr_bit_is_set(params, SPR_THREAD_SCOPE_SYSTEM)) {
        if (pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM) != 0) {
            return spr_get_errno();
        }
    }
    else if (spr_bit_is_set(params, SPR_THREAD_SCOPE_PROCESS)) {
        if (pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS) != 0) {
            return spr_get_errno();
        }
    }

    if (spr_bit_is_set(params, SPR_THREAD_CREATE_DETACHED)) {
        if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0) {
            return spr_get_errno();
        }
    }
    else if (spr_bit_is_set(params, SPR_THREAD_CREATE_JOINABLE)) {
        if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE) != 0) {
            return spr_get_errno();
        }
    }

    if (stack_size) {
        if (pthread_attr_setstacksize(&attr, stack_size) != 0) {
            return spr_get_errno();
        }
    }

    if (pthread_create(&(thread->handle), &attr, start_routine, arg) != 0) {
        return spr_get_errno();
    }

    if (pthread_attr_destroy(&attr) != 0) {
        /* Ignore error here */
    }

    return SPR_OK;
}

spr_err_t
spr_thread_join(spr_thread_t *thread)
{
    if (pthread_join(thread->handle, NULL) != 0) {
        return spr_get_errno();
    }
    return SPR_OK;
}

spr_err_t
spr_thread_cancel(spr_thread_t *thread)
{
    if (pthread_cancel(thread->handle) != 0) {
        return spr_get_errno();
    }

    return SPR_OK;
}

void
spr_thread_fini(spr_thread_t *thread)
{
    (void) thread;
}

#if (SPR_LINUX)

spr_tid_t
spr_thread_current_tid(void)
{
    return syscall(SYS_gettid);
}

#elif (SPR_FREEBSD)

spr_tid_t
spr_thread_current_tid(void)
{
    return pthread_getthreadid_np();
}

#elif (SPR_DARWIN)

spr_tid_t
spr_thread_current_tid(void)
{
    uint64_t tid;

    pthread_threadid_np(NULL, &tid);
    return tid;
}

#else

spr_tid_t
spr_thread_current_tid(void)
{
    return (uint64_t) pthread_self();
}
#endif


#elif (SPR_WIN32)

spr_err_t
spr_thread_init(spr_thread_t *thread, spr_bitfield_t params,
    size_t stack_size, spr_int_t prio, spr_thread_function_t start_routine,
    void *arg)
{
    HANDLE *t;

    (void) params;

    t = CreateThread(NULL, stack_size, start_routine, arg, 0, NULL);
    if (t == NULL) {
        return spr_get_errno();
    }

    if (SetThreadPriority(t, prio) == 0) {
        return spr_get_errno();
    } 

    thread->handle = t;

    return SPR_OK;
}

spr_err_t
spr_thread_join(spr_thread_t *thread)
{
    if (WaitForSingleObject(thread->handle, INFINITE) != WAIT_OBJECT_0) {
        return spr_get_errno();
    }

    return SPR_OK;
}

spr_err_t
spr_thread_cancel(spr_thread_t *thread)
{
    if (TerminateThread(thread->handle, 0) == 0) {
        return spr_get_errno();
    }

    return SPR_OK;
}

void
spr_thread_fini(spr_thread_t *thread)
{
    CloseHandle(thread->handle);
}

spr_tid_t
spr_thread_current_tid(void)
{
    return GetCurrentThreadId();
}

#endif
