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
#include "spr_mutex.h"
#include "spr_errno.h"

#if (SPR_POSIX)

spr_err_t
spr_mutex_init(spr_mutex_t *mutex, spr_bitfield_t params)
{
    pthread_mutexattr_t attr;

    if (pthread_mutexattr_init(&attr) != 0) {
        return spr_get_errno();
    }

    if (spr_bit_is_set(params, SPR_MUTEX_SHARED)) {
        if (pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) != 0) {
            pthread_mutexattr_destroy(&attr);
            return spr_get_errno();
        }
    }
    else if (spr_bit_is_set(params, SPR_MUTEX_PRIVATE)) {
        if (pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_PRIVATE) != 0) {
            pthread_mutexattr_destroy(&attr);
            return spr_get_errno();
        }
    }

    if (spr_bit_is_set(params, SPR_MUTEX_RECURSIVE)) {
        if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0) {
            pthread_mutexattr_destroy(&attr);
            return spr_get_errno();
        }
    }

    if (pthread_mutex_init(&(mutex->handle), &attr) != 0) {
        pthread_mutexattr_destroy(&attr);
        return spr_get_errno();
    }

    pthread_mutexattr_destroy(&attr);

    return SPR_OK;
}

spr_err_t
spr_mutex_lock(spr_mutex_t *mutex)
{
    if (pthread_mutex_lock(&(mutex->handle)) != 0) {
        return spr_get_errno();
    }

    return SPR_OK;
}

spr_err_t
spr_mutex_trylock(spr_mutex_t *mutex)
{
    if (pthread_mutex_trylock(&(mutex->handle)) != 0) {
        return SPR_BUSY;
    }
    return SPR_OK;
}

spr_err_t
spr_mutex_unlock(spr_mutex_t *mutex)
{
    if (pthread_mutex_unlock(&(mutex->handle)) != 0) {
        return spr_get_errno();
    }

    return SPR_OK;
}

void
spr_mutex_fini(spr_mutex_t *mutex)
{
    pthread_mutex_destroy(&(mutex->handle));
}


#elif (SPR_WIN32)

spr_err_t
spr_mutex_init(spr_mutex_t *mutex, spr_bitfield_t params)
{
    HANDLE handle;

    (void) params;

    if (spr_bit_is_set(params, SPR_MUTEX_RECURSIVE)) {
        InitializeCriticalSection(&mutex->section);
        mutex->handle = NULL;
        mutex->type = spr_mutex_critical_section;
    }
    else {
        handle = CreateMutex(NULL, FALSE, NULL);
        if (handle == NULL) {
            return spr_get_errno();
        }
        mutex->handle = handle;
        mutex->type = spr_mutex_mutex;
    }
    
    return SPR_OK;
}

spr_err_t
spr_mutex_lock(spr_mutex_t *mutex)
{
    if (mutex->type == spr_mutex_critical_section) {
        EnterCriticalSection(&mutex->section);
    }
    else {
        if (WaitForSingleObject(mutex->handle, INFINITE) != WAIT_OBJECT_0) {
            return spr_get_errno();
        }
    }

    return SPR_OK;
}

spr_err_t
spr_mutex_trylock(spr_mutex_t *mutex)
{
    if (mutex->type == spr_mutex_critical_section) {
        if (TryEnterCriticalSection(&mutex->section) == 0) {
            return SPR_BUSY;
        }
    }
    else {
        if (WaitForSingleObject(mutex->handle, 0) != WAIT_OBJECT_0) {
            return SPR_BUSY;
        }
    }

    return SPR_OK;
}

spr_err_t
spr_mutex_unlock(spr_mutex_t *mutex)
{
    if (mutex->type == spr_mutex_critical_section) {
        LeaveCriticalSection(&mutex->section);
    }
    else {
        if (ReleaseMutex(mutex->handle) == 0) {
            return spr_get_errno();
        }
    }
    return SPR_OK;
}

void
spr_mutex_fini(spr_mutex_t *mutex)
{
    if (mutex->type == spr_mutex_critical_section) {
        DeleteCriticalSection(&mutex->section);
    }
    else {
        CloseHandle(mutex->handle);
    }
}

#endif
