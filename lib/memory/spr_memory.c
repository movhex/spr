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
#include "spr_memory.h"

#if defined(__GNUC__)
#define memory_barrier() __sync_synchronize()
#elif defined(__sparc) || defined(__sparc__)
#define memory_barrier() __asm (".volatile"); __asm (".nonvolatile")
#else
#define memory_barrier()
/* TODO: etc */
#endif

static uint64_t spr_memory_counter_value;


void *
spr_malloc(size_t size)
{
    void *mem;

    mem = malloc(size);
    if (mem) {
        spr_memory_counter_value += size;
    }

    return mem;
}

void *
spr_calloc(size_t size)
{
    void *mem;

    mem = spr_malloc(size);
    if (!mem) {
        return mem;
    }
    spr_memset(mem, 0, size);
    return mem;
}

void
spr_free(void *mem)
{
    free(mem);
}

size_t
spr_get_memory_counter(void)
{
    return spr_memory_counter_value;
}

void
spr_explicit_memzero(void *buf, size_t n)
{
    spr_memset(buf, 0, n);
    memory_barrier();
}


#if (SPR_POSIX)

size_t
spr_get_page_size(void)
{
    size_t page_size;

#if defined(_SC_PAGESIZE)
    page_size = sysconf(_SC_PAGESIZE);
#else
#error "Failed to determine page size"
#endif

    return page_size;
}

#elif (SPR_WIN32)

size_t
spr_get_page_size(void)
{
    SYSTEM_INFO si;
    size_t page_size;

    GetSystemInfo(&si);
    page_size = si.dwPageSize;

    return page_size;
}

#endif
