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

#ifndef INCLUDED_SPR_MEMORY_H
#define INCLUDED_SPR_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

#define SPR_PTR_SIZE                 __SIZEOF_POINTER__
#define SPR_PTR_WIDTH                (8 * SPR_PTR_SIZE)
#define SPR_ALIGN_SIZE               sizeof(uintptr_t)
#define SPR_PAGE_SIZE                spr_get_page_size()

#define spr_align(p, b)  (((p) + ((b) - 1)) & ~((b) - 1))
#define spr_align_default(p)         spr_align(p, SPR_ALIGN_SIZE)

#define spr_memset(buf, c, n)        memset(buf, c, n)
#define spr_memzero(buf, n)          spr_memset(buf, 0, n)
#define spr_memcmp(s1, s2, n)        memcmp(s1, s2, n)
#define spr_memcpy(dst, src, n)      memcpy(dst, src, n)
#define spr_memmove(dst, src, n)     memmove(dst, src, n)


void *spr_malloc(size_t size);
void *spr_calloc(size_t size);
void spr_free(void *mem);
size_t spr_get_memory_counter(void);

void spr_explicit_memzero(void *buf, size_t n);
size_t spr_get_page_size(void);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_SPR_MEMORY_H */
