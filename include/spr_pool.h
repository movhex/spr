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

#ifndef INCLUDED_SPR_POOL_H
#define INCLUDED_SPR_POOL_H

#include "spr_portable.h"

#ifdef __cplusplus
extern "C" {
#endif

#define spr_pool_cleanup_add(pool, data, handler) \
    spr_pool_cleanup_add1(pool, data, (spr_cleanup_handler_t) handler)

#define spr_pool_cleanup_run(pool, data, handler) \
    spr_pool_cleanup_run1(pool, data, (spr_cleanup_handler_t) handler)

#define spr_pool_cleanup_remove(pool, data, handler) \
    spr_pool_cleanup_remove1(pool, data, (spr_cleanup_handler_t) handler)

typedef struct spr_pool_s spr_pool_t;
typedef void (*spr_cleanup_handler_t)(void *data);

spr_pool_t *spr_pool_create(size_t size, spr_pool_t *parent);
spr_err_t spr_pool_create1(spr_pool_t **newpool, size_t size,
    spr_pool_t *parent);
void spr_pool_add_child(spr_pool_t *parent, spr_pool_t *new_child);
void *spr_palloc(spr_pool_t *pool, size_t size);
void *spr_pcalloc(spr_pool_t *pool, size_t size);
void spr_pool_cleanup_add1(spr_pool_t *pool, void *data,
    spr_cleanup_handler_t handler);
void spr_pool_cleanup_run1(spr_pool_t *pool, void *data,
    spr_cleanup_handler_t handler);
void spr_pool_cleanup_remove1(spr_pool_t *pool, void *data,
    spr_cleanup_handler_t handler);
void spr_pool_clear(spr_pool_t *pool);
void spr_pool_destroy(spr_pool_t *pool);

size_t spr_pool_get_size(spr_pool_t *pool);
size_t spr_pool_get_free_size(spr_pool_t *pool);
size_t spr_pool_get_total_size(spr_pool_t *pool);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_SPR_POOL_H */
