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
#include "spr_semaphore.h"
#include "spr_errno.h"

#if (SPR_POSIX)

spr_err_t
spr_semaphore_init(spr_semaphore_t *semaphore, spr_uint_t value,
    spr_uint_t maxval, spr_bitfield_t params)
{
    spr_uint_t shared;

    (void) maxval;
    shared = 0;

    if (spr_bit_is_set(params, SPR_SEMAPHORE_SHARED)) {
        shared = 1;
        /* Check conflict of shared params */
        if (params & (SPR_SEMAPHORE_PRIVATE)) {
            return SPR_FAILED;
        }
    }

    if (sem_init(&(semaphore->handle), shared, value) != 0) {
        return spr_get_errno();
    }
    return SPR_OK;
}

spr_err_t
spr_semaphore_wait(spr_semaphore_t *semaphore)
{
    if (sem_wait(&(semaphore->handle)) != 0) {
        return spr_get_errno();
    }
    return SPR_OK;
}

spr_err_t
spr_semaphore_trywait(spr_semaphore_t *semaphore)
{
    if (sem_trywait(&(semaphore->handle)) != 0) {
        return SPR_BUSY;
    }
    return SPR_OK;
}

spr_err_t
spr_semaphore_post(spr_semaphore_t *semaphore)
{
    if (sem_post(&(semaphore->handle)) != 0) {
        return spr_get_errno();
    }
    return SPR_OK;
}

void
spr_semaphore_fini(spr_semaphore_t *semaphore)
{
    sem_destroy(&(semaphore->handle));
}

#elif (SPR_WIN32)

spr_err_t
spr_semaphore_init(spr_semaphore_t *semaphore, spr_uint_t value,
    spr_uint_t maxval, spr_bitfield_t params)
{
    HANDLE s;
    SECURITY_ATTRIBUTES attr;
    spr_uint_t shared;

    (void) params;

    shared = 1;

#if 0
    if (spr_bit_is_set(params, SPR_SEMAPHORE_SHARED)) {
        shared = 1;
        /* Check conflict of shared params */
        if (params & (SPR_SEMAPHORE_PRIVATE)) {
            return spr_get_errno();
        }
    }
#endif

    attr.nLength = sizeof(SECURITY_ATTRIBUTES);
    attr.lpSecurityDescriptor = NULL;
    attr.bInheritHandle = shared;

    s = CreateSemaphoreA(&attr, value, maxval, NULL);
    if (s == NULL) {
        return spr_get_errno();
    }

    semaphore->handle = s;

    return SPR_OK;
}

spr_err_t
spr_semaphore_wait(spr_semaphore_t *semaphore)
{
    if (WaitForSingleObject(semaphore->handle, INFINITE) != WAIT_OBJECT_0) {
        return spr_get_errno();
    }
    return SPR_OK;
}

spr_err_t
spr_semaphore_trywait(spr_semaphore_t *semaphore)
{
    if (WaitForSingleObject(semaphore->handle, 0) != WAIT_OBJECT_0) {
        return SPR_BUSY;
    }
    return SPR_OK;
}

spr_err_t
spr_semaphore_post(spr_semaphore_t *semaphore)
{
    if (ReleaseSemaphore(semaphore->handle, 1, NULL) == 0) {
        return spr_get_errno();
    }
    return SPR_OK;
}

void
spr_semaphore_fini(spr_semaphore_t *semaphore)
{
    CloseHandle(semaphore->handle);
}

#endif
