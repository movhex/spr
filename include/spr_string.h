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

#ifndef INCLUDED_SPR_STRING_H
#define INCLUDED_SPR_STRING_H

#include "spr_portable.h"
#include "spr_pool.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPR_NULL                     0x0
#define SPR_TAB                      0x9
#define SPR_LF                       0xA
#define SPR_CR                       0xD
#define SPR_SPACE                    0x20

#define SPR_LF_STR                   "\n"
#define SPR_CR_STR                   "\r"
#define SPR_CRLF_STR                 "\r\n"

#if (SPR_POSIX)
#define SPR_EOL_STR                  SPR_LF_STR
#elif (SPR_WIN32)
#define SPR_EOL_STR                  SPR_CRLF_STR
#endif

#define spr_strcmp(s1, s2)           strcmp(s1, s2)
#define spr_strncmp(s1, s2, n)       strncmp(s1, s2, n)
#define spr_snprintf(str, size, ...) \
    snprintf(str, size, __VA_ARGS__)
#define spr_vsnprintf(str, size, ...) \
    vsnprintf(str, size, __VA_ARGS__)

#define spr_isdigit(c) \
    ((c >= '0') && (c <= '9'))

#define spr_isalpha(c) \
    (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')))

#define spr_isascii(c) \
    ((((int8_t) c) >= 0x0) && (((uint8_t) c) <= 0x7f))

#define spr_tolower(c) \
    ((char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c))

#define spr_toupper(c) \
    ((char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c))


#define spr_value_helper(x)          #x
#define spr_value(x)                 spr_value_helper(x)
#define spr_stringify(x)             spr_value_helper(x)

#define spr_pstrcat(...)             spr_pstrcat1(__VA_ARGS__, NULL)

spr_int_t spr_atoi(const char *str, size_t n);
ssize_t spr_atosz(const char *str, size_t n);
size_t spr_strlen(const char *str);
size_t spr_strnlen(const char *str, size_t n);
spr_int_t spr_strcasecmp(char *str1, char *str2);
spr_int_t spr_strncasecmp(char *str1, char *str2, size_t n);
char *spr_strchr(const char *str, int c);
char *spr_strrchr(const char *str, int c);
char *spr_pstrdup(spr_pool_t *pool, const char *str);
char *spr_pstrndup(spr_pool_t *pool, const char *str, size_t n);
char *spr_pstrcat1(spr_pool_t *pool, ...);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_SPR_STRING_H */
