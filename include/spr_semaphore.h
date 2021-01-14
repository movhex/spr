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

#ifndef INCLUDED_SPR_SEMAPHORE_H
#define INCLUDED_SPR_SEMAPHORE_H

#include "spr_portable.h"
#include "spr_bitfield.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Semaphore specific parameters */
#define SPR_SEMAPHORE_PRIVATE        0x00000001
#define SPR_SEMAPHORE_SHARED         0x00000002

#if (SPR_POSIX)

struct spr_semaphore_s {
    sem_t handle;
};

#elif (SPR_WIN32)

struct spr_semaphore_s {
    HANDLE handle;
};

#endif

spr_err_t spr_semaphore_init(spr_semaphore_t *semaphore,
    spr_uint_t value, spr_uint_t maxval, spr_bitfield_t params);
spr_err_t spr_semaphore_wait(spr_semaphore_t *semaphore);
spr_err_t spr_semaphore_trywait(spr_semaphore_t *semaphore);
spr_err_t spr_semaphore_post(spr_semaphore_t *semaphore);
void spr_semaphore_fini(spr_semaphore_t *semaphore);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_SPR_SEMAPHORE_H */
