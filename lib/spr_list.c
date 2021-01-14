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

#include "spr_list.h"
#include "spr_errno.h"

#define SPR_LIST_INITIAL_SIZE  10


spr_err_t
spr_list_create1(spr_list_t **out_list, spr_pool_t *pool)
{
    spr_list_t *list;

    list = spr_pcalloc(pool, sizeof(spr_list_t));
    if (!list) {
        return spr_get_errno();
    }

    /*
     * Next fields set by spr_pcalloc()
     *
     * list->head = NULL;
     * list->tail = NULL;
     * list->free_nodes = NULL;
     * list->size = 0;
     * list->capacity = 0;
     *
     */

    list->pool = pool;

    *out_list = list;

    return SPR_OK;
}

spr_list_t *
spr_list_create(spr_pool_t *pool)
{
    spr_list_t *list;

    if (spr_list_create1(&list, pool) != SPR_OK) {
        return NULL;
    }

    return list;
}

spr_err_t
spr_list_push_back(spr_list_t *list, void *data)
{
    spr_list_node_t *nodes, *node, *temp;
    spr_uint_t i;
    size_t alloc_size;

    if (list->free_nodes) {
        node = list->free_nodes;
        list->free_nodes = node->next;
        list->capacity -= 1;
    }
    else {
        alloc_size = sizeof(spr_list_node_t) * SPR_LIST_INITIAL_SIZE;
        nodes = spr_palloc(list->pool, alloc_size);
        if (!nodes) {
            return spr_get_errno();
        }

        node = &nodes[0];
        nodes = &nodes[1];

        for (i = 0; i < (SPR_LIST_INITIAL_SIZE - 1); ++i) {
            temp = list->free_nodes;
            list->free_nodes = &nodes[i];
            list->free_nodes->next = temp;
        }
        list->capacity = SPR_LIST_INITIAL_SIZE - 1;
    }

    node->next = NULL;
    node->data = data;

    if (list->tail) {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }
    else {
        list->head = node;
        list->tail = node;
    }

    list->size += 1;

    return SPR_OK;
}

spr_err_t
spr_list_push_front(spr_list_t *list, void *data)
{
    spr_list_node_t *nodes, *node, *temp;
    spr_uint_t i;
    size_t alloc_size;

    if (list->free_nodes) {
        node = list->free_nodes;
        list->free_nodes = node->next;
        list->capacity -= 1;
    }
    else {
        alloc_size = sizeof(spr_list_node_t) * SPR_LIST_INITIAL_SIZE;
        nodes = spr_palloc(list->pool, alloc_size);
        if (!nodes) {
            return spr_get_errno();
        }

        node = &nodes[0];
        nodes = &nodes[1];

        for (i = 0; i < (SPR_LIST_INITIAL_SIZE - 1); ++i) {
            temp = list->free_nodes;
            list->free_nodes = &nodes[i];
            list->free_nodes->next = temp;
        }
        list->capacity = SPR_LIST_INITIAL_SIZE - 1;
    }

    node->prev = NULL;
    node->data = data;

    if (list->head) {
        list->head->prev = node;
        node->next = list->head;
        list->head = node;
    }
    else {
        list->head = node;
        list->tail = node;
    }

    list->size += 1;

    return SPR_OK;
}

spr_err_t
spr_list_reserve(spr_list_t *list, size_t n)
{
    spr_list_node_t *nodes, *temp;
    spr_uint_t i;
    size_t alloc_size;

    alloc_size = sizeof(spr_list_node_t) * n;
    nodes = spr_palloc(list->pool, alloc_size);
    if (!nodes) {
        return spr_get_errno();
    }

    for (i = 0; i < n; ++i) {
        temp = list->free_nodes;
        list->free_nodes = &nodes[i];
        list->free_nodes->next = temp;
    }
    list->capacity += n;

    return SPR_OK;
}

spr_err_t
spr_list_remove1(spr_list_t *list, spr_list_node_t *node)
{
    spr_list_node_t *founded_node, *temp1;

    temp1 = list->head;

    /* If first element */
    if (list->head == node) {
        founded_node = list->head;
        if (list->head->next) {
            list->head->next->prev = NULL;
        }
        list->head = list->head->next;
        list->size -= 1;
    }
    /* If the last element */
    else if (list->tail == node) {
        founded_node = list->tail;
        if (list->tail->prev) {
            list->tail->prev->next = NULL;
        }
        list->tail = list->tail->prev;
        list->size -= 1;
    }
    else {
        temp1 = temp1->next;
        while (temp1 != node) {
            temp1 = temp1->next;
            if (temp1 == list->tail) {
                return SPR_NOT_FOUND;
            }
        }
        temp1->next->prev = temp1->prev;
        temp1->prev->next = temp1->next;
        founded_node = temp1;
        list->size -= 1;
    }

    temp1 = list->free_nodes;
    list->free_nodes = founded_node;
    founded_node->next = temp1;

    list->capacity += 1;

    return SPR_OK;
}

spr_err_t
spr_list_remove(spr_list_t *list, void *data)
{
    spr_list_node_t *founded_node, *temp1;

    temp1 = list->head;

    /* If first element */
    if (list->head->data == data) {
        founded_node = list->head;
        if (list->head->next) {
            list->head->next->prev = NULL;
        }
        list->head = list->head->next;
        list->size -= 1;
    }
    /* If the last element */
    else if (list->tail->data == data) {
        founded_node = list->tail;
        if (list->tail->prev) {
            list->tail->prev->next = NULL;
        }
        list->tail = list->tail->prev;
        list->size -= 1;
    }
    else {
        temp1 = temp1->next;
        while (temp1->data != data) {
            temp1 = temp1->next;
            if (temp1 == list->tail) {
                return SPR_NOT_FOUND;
            }
        }
        temp1->next->prev = temp1->prev;
        temp1->prev->next = temp1->next;
        founded_node = temp1;
        list->size -= 1;
    }

    temp1 = list->free_nodes;
    list->free_nodes = founded_node;
    founded_node->next = temp1;

    list->capacity += 1;

    return SPR_OK;
}

void
spr_list_clear(spr_list_t *list)
{
    spr_list_node_t *node, *temp;

    node = list->head;

    while (node) {
        temp = node->next;
        spr_list_remove(list, node);
        node = temp;
    }
}
