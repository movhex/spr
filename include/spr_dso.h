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

#ifndef INCLUDED_SPR_DSO_H
#define INCLUDED_SPR_DSO_H

#include "spr_portable.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (SPR_POSIX)

#define spr_dlopen(path)           dlopen((char *) path, RTLD_NOW|RTLD_GLOBAL)
#define spr_dlsym(handle, symbol)  dlsym(handle, symbol)
#define spr_dlclose(handle)        dlclose(handle)

const char *spr_dlerror(char *buf, size_t bufsize);


#elif (SPR_WIN32)

#define spr_dlopen(path)           LoadLibrary((char *) path)
#define spr_dlsym(handle, symbol)  (void *) GetProcAddress(handle, symbol)
#define spr_dlclose(handle)        (FreeLibrary(handle) ? 0 : -1)

const char *spr_dlerror(char *buf, size_t bufsize);

#endif

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_SPR_DSO_H */
