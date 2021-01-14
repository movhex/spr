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
#include "spr_array.h"
#include "spr_memory.h"
#include "spr_errno.h"

spr_array_t *
spr_array_create(spr_pool_t *pool, size_t size, size_t n)
{
    spr_array_t *array;
    void *data;

    array = spr_pcalloc(pool, sizeof(spr_array_t));

    data = spr_pcalloc(pool, size * n);

    /*
     * Next fields set by spr_pcalloc()
     * 
     * array->n_current = 0;
     * 
     */

    array->size = size;
    array->n_total = n;
    array->data = data;

    return array;
}

spr_err_t
spr_array_push(spr_array_t *array, void *data)
{
    void *mem;

    if (array->n_current >= array->n_total) {
        return SPR_FAILED;
    }

    mem = ((uint8_t *) array->data) + (array->n_current * array->size);

    spr_memmove(mem, data, array->size);

    array->size += 1;

    return SPR_OK;
}

void
spr_array_clear(spr_array_t *array)
{
    array->n_current = 0;
    array->data = NULL;
}
