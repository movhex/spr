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

#ifndef INCLUDED_SPR_LIMITS_H
#define INCLUDED_SPR_LIMITS_H

#ifdef __cplusplus
extern "C" {
#endif

#define SPR_INT32_MAX_VALUE          (uint32_t) 0x7fffffff
#define SPR_UINT32_MAX_VALUE         (uint32_t) 0xffffffff

#define SPR_INT64_MAX_VALUE          (uint64_t) 0x7fffffffffffffff
#define SPR_UINT64_MAX_VALUE         (uint64_t) 0xffffffffffffffff


#define SPR_INT32_MAX_LEN            (sizeof("-2147483648") - 1)
#define SPR_UINT32_MAX_LEN           (sizeof("4294967295") - 1)

#define SPR_INT64_MAX_LEN            (sizeof("-9223372036854775807") - 1)
#define SPR_UINT64_MAX_LEN           (sizeof("18446744073709551615") - 1)

#if (SPR_PTR_SIZE == 4)

#define SPR_INT_T_MAX                SPR_INT32_MAX_VALUE
#define SPR_UINT_T_MAX               SPR_UINT32_MAX_VALUE
#define SPR_INT_T_LEN                SPR_INT32_MAX_LEN
#define SPR_UINT_T_LEN               SPR_UINT32_MAX_LEN

#define SPR_SIZE_T_MAX               SPR_UINT32_MAX_VALUE
#define SPR_SSIZE_T_MAX              SPR_INT32_MAX_VALUE

#define SPR_FMT_INT_T                "%d"
#define SPR_FMT_UINT_T               "%u"
#define SPR_FMT_SSIZE_T              "%d"
#define SPR_FMT_SIZE_T               "%u"

#else
#define SPR_INT_T_MAX                SPR_INT64_MAX_VALUE
#define SPR_UINT_T_MAX               SPR_UINT64_MAX_VALUE
#define SPR_INT_T_LEN                SPR_INT64_MAX_LEN
#define SPR_UINT_T_LEN               SPR_UINT64_MAX_LEN

#define SPR_SIZE_T_MAX               SPR_UINT64_MAX_VALUE
#define SPR_SSIZE_T_MAX              SPR_INT64_MAX_VALUE

#if (SPR_POSIX)
#define SPR_FMT_INT_T                "%ld"
#define SPR_FMT_UINT_T               "%lu"
#define SPR_FMT_SSIZE_T              "%ld"
#define SPR_FMT_SIZE_T               "%lu"

#elif (SPR_WIN32)

#if defined(__USE_MINGW_ANSI_STDIO)
#define SPR_FMT_INT_T                "%lld"
#define SPR_FMT_UINT_T               "%llu"
#define SPR_FMT_SSIZE_T              "%lld"
#define SPR_FMT_SIZE_T               "%llu"
#else
#define SPR_FMT_INT_T                "%I64d"
#define SPR_FMT_UINT_T               "%I64u"
#define SPR_FMT_SSIZE_T              "%I64d"
#define SPR_FMT_SIZE_T               "%I64u"
#endif

#endif

#endif

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_SPR_LIMITS_H */
