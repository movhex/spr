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
#include "spr_pool.h"
#include "spr_memory.h"
#include "spr_errno.h"
#include "spr_thread.h"
#include "spr_mutex.h"


#define SPR_MIN_ORDER  1
#define SPR_MIN_ALLOC  (SPR_ALIGN_SIZE << SPR_MIN_ORDER)

#define SPR_SIZEOF_MEMNODE_T_ALIGN \
    spr_align_default(sizeof(spr_memnode_t))
#define SPR_SIZEOF_MEMPOOL_T_ALIGN \
    spr_align_default(sizeof(spr_pool_t))
#define SPR_SIZEOF_MUTEX_T_ALIGN \
    spr_align_default(sizeof(spr_mutex_t))

/* 
 * Slot  0: size 4096
 * Slot  1: size 8192
 * Slot  2: size 12288
 * ...
 * Slot 19: size 81920
 */
#define SPR_MAX_POOL_SLOT  20


typedef struct spr_memnode_s spr_memnode_t;
typedef struct spr_cleanup_node_s spr_cleanup_node_t;

struct spr_memnode_s {
    spr_memnode_t *next;
    uint8_t *begin;
    uint8_t *first_avail;
    size_t size;
    size_t size_avail; /* To increase search speed */
    size_t dealloc_size;
};

struct spr_cleanup_node_s {
    spr_cleanup_node_t *next;
    void *data;
    spr_cleanup_handler_t handler;
};

struct spr_pool_s {
    spr_memnode_t *nodes[SPR_MAX_POOL_SLOT];
    spr_pool_t *parent;
    spr_pool_t *brother;
    spr_pool_t *child;

    spr_cleanup_node_t *cleanups;
    spr_cleanup_node_t *free_cleanups;

#if (SPR_POOL_THREAD_SAFETY)
    spr_mutex_t *mutex;
    spr_thread_handle_t owner;
#endif
};


static size_t
spr_align_allocation(size_t size)
{
    size_t align_size;

    align_size = spr_align_default(size);
    if (align_size < size) {
        return 0;
    }
    if (align_size < SPR_MIN_ALLOC) {
        align_size = SPR_MIN_ALLOC;
    }
    return align_size;
}

static size_t
spr_get_npages(size_t size)
{
    size_t total_size;

    total_size = size + SPR_SIZEOF_MEMNODE_T_ALIGN;
    if (total_size < SPR_PAGE_SIZE) {
        return 1;
    }
    else {
        return ((total_size / SPR_PAGE_SIZE) + 1);
    }
}

static spr_memnode_t *
spr_memnode_allocate(size_t size)
{
    spr_memnode_t *node;
    uint8_t *mem;

#if (SPR_HAVE_MMAP && SPR_POOL_USES_MMAP)
    mem = mmap(NULL, size, PROT_READ|PROT_WRITE,
                                MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        return NULL;
    }
#else
    mem = spr_malloc(size);
    if (mem == NULL) {
        return NULL;
    }
#endif

    node = (spr_memnode_t *) mem;

    /* Initialization of memnode */
    node->next = NULL;
    node->begin = mem + SPR_SIZEOF_MEMNODE_T_ALIGN;
    node->first_avail = node->begin;
    node->size = size - SPR_SIZEOF_MEMNODE_T_ALIGN;
    node->size_avail = node->size;
    node->dealloc_size = size;

    return node;
}

static void
spr_memnode_deallocate(spr_memnode_t *node)
{
#if (SPR_HAVE_MMAP && SPR_POOL_USES_MMAP)
    munmap(node, node->dealloc_size);
#else
    spr_free(node);
#endif
}

static void
spr_pool_cleanup_run_all(spr_pool_t *pool)
{
    spr_cleanup_node_t *temp1, *temp2;

    temp1 = pool->cleanups;

    while (temp1) {

        temp2 = temp1;
        temp1 = temp1->next;

        temp2->handler(temp2->data);

        /* Reserve node */
        temp2->next = pool->free_cleanups;
        pool->free_cleanups = temp2;
    }

    pool->cleanups = NULL;
}

void
spr_pool_add_child(spr_pool_t *parent, spr_pool_t *new_child)
{
    spr_pool_t *temp;

    new_child->parent = parent;

    /* New child is not first */
    if (parent->child) {
        temp = parent->child;
        parent->child = new_child;
        new_child->brother = temp;
    }
    /* New child is first */
    else {
        parent->child = new_child;
        new_child->brother = NULL;
    }
}

spr_err_t
spr_pool_create1(spr_pool_t **newpool, size_t size, spr_pool_t *parent)
{
    spr_memnode_t *node;
    spr_pool_t *pool;
    size_t npages, index, align_size;

#if (SPR_POOL_THREAD_SAFETY)
    spr_mutex_t *mutex;
#endif
    spr_err_t err;

    node = NULL;

    align_size = spr_align_allocation(size);
    if (align_size == 0) {
        err = SPR_FAILED;
        goto failed;
    }

    align_size += SPR_SIZEOF_MEMPOOL_T_ALIGN;

    npages = spr_get_npages(align_size);
    index = npages - 1;
    if (index > SPR_MAX_POOL_SLOT) {
        err = SPR_FAILED;
        goto failed;
    }

    node = spr_memnode_allocate(npages * SPR_PAGE_SIZE);
    if (!node) {
        err = spr_get_errno();
        goto failed;
    }

    /* Reserve memory for service info */
    pool = (spr_pool_t *) node->begin;
    spr_memset(pool, 0, sizeof(spr_pool_t));
    node->begin += SPR_SIZEOF_MEMPOOL_T_ALIGN;
    node->size -= SPR_SIZEOF_MEMPOOL_T_ALIGN;

#if (SPR_POOL_THREAD_SAFETY)

    if (parent) {
        mutex = parent->mutex;
    }
    else {
        mutex = (spr_mutex_t *) node->begin;
        node->begin += SPR_SIZEOF_MUTEX_T_ALIGN;
        node->size -= SPR_SIZEOF_MUTEX_T_ALIGN;

        err = spr_mutex_init(mutex, SPR_MUTEX_SHARED|SPR_MUTEX_RECURSIVE);
        if (err != SPR_OK) {
            goto failed;
        }
    }

    pool->mutex = mutex;
    pool->owner = spr_thread_current_handle();

#endif

    /* Modify memnode service info */
    node->first_avail = node->begin;
    node->size_avail = node->size;

    /* Registaration of memnode */
    (pool->nodes)[index] = node;
    node->next = NULL;

    if (parent) {
        spr_pool_add_child(parent, pool);
    }

    *newpool = pool;

    return SPR_OK;

failed:
    if (node) {
        spr_memnode_deallocate(node);
    }

    return err;
}

spr_pool_t *
spr_pool_create(size_t size, spr_pool_t *parent)
{
    spr_pool_t *pool;

    if (spr_pool_create1(&pool, size, parent) != SPR_OK) {
        return NULL;
    }
    return pool;
}

void *
spr_palloc(spr_pool_t *pool, size_t size)
{
    spr_memnode_t *node, *temp;
    spr_uint_t i;
    size_t align_size, npages, index;
    void *mem;

    node = NULL;
    mem = NULL;

    align_size = spr_align_allocation(size);
    if (!align_size) {
        goto failed;
    }

    /*
     * First see if there are any nodes in the area we know
     * our node will fit into
     */
    npages = spr_get_npages(align_size);
    index = npages - 1;

    /*
     * When the requested allocation is too large to fit into a block,
     * the request is forwarded to the system allocator and the returned
     * pointer is stored in the pool for further deallocation
     */
    if (index > SPR_MAX_POOL_SLOT) {
        mem = spr_malloc(align_size);
        if (mem) {
            spr_pool_cleanup_add(pool, mem, spr_free);
        }
        goto done;
    }

#if (SPR_POOL_THREAD_SAFETY)
    spr_mutex_lock(pool->mutex);
#endif

    /* Try to find suitable node */
    for (i = index; i < SPR_MAX_POOL_SLOT; ++i) {
        node = (pool->nodes)[i];
        while (node) {
            if (node->size_avail >= align_size) {
                node->size_avail -= align_size;
                mem = node->first_avail;
                node->first_avail += align_size;
                goto done;
            }
            node = node->next;
        }
    }

    /* If we haven't got a suitable node, allocate a new one */
    if (!node) {
        node = spr_memnode_allocate(npages * SPR_PAGE_SIZE);
        if (!node) {
            goto failed;
        }

        /* Registaration of memnode */
        temp = (pool->nodes)[index];
        (pool->nodes)[index] = node;
        node->next = temp;

        node->size_avail -= align_size;
        mem = node->first_avail;
        node->first_avail += align_size;
        goto done;
    }

done:
#if (SPR_POOL_THREAD_SAFETY)
    spr_mutex_unlock(pool->mutex);
#endif
    return mem;

failed:
#if (SPR_POOL_THREAD_SAFETY)
    spr_mutex_unlock(pool->mutex);
#endif
    return NULL;
}

void *
spr_pcalloc(spr_pool_t *pool, size_t size)
{
    void *mem;

    mem = spr_palloc(pool, size);
    if (mem) {
        spr_memset(mem, 0, size);
    }
    return mem;
}

void
spr_pool_cleanup_add1(spr_pool_t *pool, void *data,
    spr_cleanup_handler_t handler)
{
    spr_cleanup_node_t *node;

#if (SPR_POOL_THREAD_SAFETY)
    spr_mutex_lock(pool->mutex);
#endif

    if (pool->free_cleanups) {
        node = pool->free_cleanups;
        pool->free_cleanups = node->next;
    }
    else {
        node = spr_palloc(pool, sizeof(spr_cleanup_node_t));
    }
    node->data = data;
    node->handler = handler;
    node->next = pool->cleanups;
    pool->cleanups = node;

#if (SPR_POOL_THREAD_SAFETY)
    spr_mutex_unlock(pool->mutex);
#endif
}

void
spr_pool_cleanup_run1(spr_pool_t *pool, void *data,
    spr_cleanup_handler_t handler)
{
    spr_cleanup_node_t *node, *prev;

    node = pool->cleanups;

#if (SPR_POOL_THREAD_SAFETY)
    spr_mutex_lock(pool->mutex);
#endif

    while (node) {
        if (node->data == data && node->handler == handler) {
            node->handler(node->data);
            break;
        }
        prev = node;
        node = node->next;
    }

    if (!node) {
        /* Not found */
        return;
    }

    if (pool->cleanups == node) {
        pool->cleanups = node->next;
    }
    else {
        prev->next = node->next;
    }

    /* Reserve node */
    node->next = pool->free_cleanups;
    pool->free_cleanups = node;

#if (SPR_POOL_THREAD_SAFETY)
    spr_mutex_unlock(pool->mutex);
#endif
}

void
spr_pool_cleanup_remove1(spr_pool_t *pool, void *data,
    spr_cleanup_handler_t handler)
{
    spr_cleanup_node_t *node, *prev;

    node = pool->cleanups;

#if (SPR_POOL_THREAD_SAFETY)
    spr_mutex_lock(pool->mutex);
#endif

    while (node) {
        if (node->data == data && node->handler == handler) {
            break;
        }
        prev = node;
        node = node->next;
    }

    if (!node) {
        /* Not found */
        return;
    }

    if (pool->cleanups == node) {
        pool->cleanups = node->next;
    }
    else {
        prev->next = node->next;
    }

    /* Reserve node */
    node->next = pool->free_cleanups;
    pool->free_cleanups = node;

#if (SPR_POOL_THREAD_SAFETY)
    spr_mutex_unlock(pool->mutex);
#endif
}


size_t
spr_pool_get_size(spr_pool_t *pool)
{
    spr_memnode_t *node;
    spr_uint_t i;
    size_t size;

    size = 0;

    for (i = 0; i < SPR_MAX_POOL_SLOT; ++i) {
        node = (pool->nodes)[i];
        while (node) {
            size += node->size;
            node = node->next;
        }
    }
    return size;
}

size_t
spr_pool_get_free_size(spr_pool_t *pool)
{
    spr_memnode_t *node;
    spr_uint_t i;
    size_t size;

    size = 0;

    for (i = 0; i < SPR_MAX_POOL_SLOT; ++i) {
        node = (pool->nodes)[i];
        while (node) {
            size += node->size_avail;
            node = node->next;
        }
    }
    return size;
}

size_t
spr_pool_get_total_size(spr_pool_t *pool)
{
    spr_memnode_t *node;
    spr_uint_t i;
    size_t size;

    size = 0;

    for (i = 0; i < SPR_MAX_POOL_SLOT; ++i) {
        node = (pool->nodes)[i];
        while (node) {
            size += SPR_PAGE_SIZE * (i + 1);
            node = node->next;
        }
    }
    return size;
}

void
spr_pool_clear(spr_pool_t *pool)
{
    spr_pool_t *temp1, *temp2;
    spr_memnode_t *node;
    spr_uint_t i;

#if (SPR_POOL_THREAD_SAFETY)
    if (!spr_thread_equal(pool->owner, spr_thread_current_handle())) {
        return;
    }

    spr_mutex_lock(pool->mutex);
#endif

    spr_pool_cleanup_run_all(pool);

    if (pool->child) {
        temp1 = pool->child;
        while (temp1) {
            temp2 = temp1;
            temp1 = temp1->brother;
            spr_pool_clear(temp2);
        }
    }

    for (i = 0; i < SPR_MAX_POOL_SLOT; ++i) {
        node = (pool->nodes)[i];
        while (node) {
            node->first_avail = node->begin;
            node->size_avail = node->size;
            node = node->next;
        }
    }

#if (SPR_POOL_THREAD_SAFETY)
    spr_mutex_unlock(pool->mutex);
#endif
}

void
spr_pool_destroy(spr_pool_t *pool)
{
    spr_pool_t *temp1, *temp2;
    spr_memnode_t *node, *temp;
    spr_uint_t i;

#if (SPR_POOL_THREAD_SAFETY)
    if (!spr_thread_equal(pool->owner, spr_thread_current_handle())) {
        return;
    }
#endif

    spr_pool_cleanup_run_all(pool);

    if (pool->child) {
        temp1 = pool->child;
        while (temp1) {
            temp2 = temp1;
            temp1 = temp1->brother;
            spr_pool_destroy(temp2);
        }
    }

    for (i = 0; i < SPR_MAX_POOL_SLOT; ++i) {
        node = (pool->nodes)[i];
        while (node) {
            temp = node;
            node = node->next;
            spr_memnode_deallocate(temp);
        }
    }
}
