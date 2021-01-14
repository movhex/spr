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

#ifndef INCLUDED_SPR_LIST_H
#define INCLUDED_SPR_LIST_H

#include "spr_pool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct spr_list_s spr_list_t;
typedef struct spr_list_node_s spr_list_node_t;

struct spr_list_node_s {
    spr_list_node_t *next;
    spr_list_node_t *prev;
    void *data;
};

struct spr_list_s {
    spr_pool_t *pool;
    spr_list_node_t *head;
    spr_list_node_t *tail;
    spr_list_node_t *free_nodes;
    size_t size;
    size_t capacity;
};

#define spr_list_first(list)         (list->head)
#define spr_list_last(list)          (list->tail)
#define spr_list_size(list)          (list->size)
#define spr_list_capacity(list)      (list->capacity)

#define spr_list_next(node)          (node->next)
#define spr_list_prev(node)          (node->prev)
#define spr_list_data(node)          (node->data)

spr_list_t *spr_list_create(spr_pool_t *pool);
spr_err_t spr_list_create1(spr_list_t **list, spr_pool_t *pool);

spr_err_t spr_list_push_back(spr_list_t *list, void *data);
spr_err_t spr_list_push_front(spr_list_t *list, void *data);
spr_err_t spr_list_remove(spr_list_t *list, void *data);
spr_err_t spr_list_remove1(spr_list_t *list, spr_list_node_t *node);

spr_err_t spr_list_reserve(spr_list_t *list, size_t n);

void spr_list_clear(spr_list_t *list);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_SPR_LIST_H */
