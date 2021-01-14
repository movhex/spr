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
#include "spr_limits.h"
#include "spr_memory.h"
#include "spr_pool.h"


spr_int_t
spr_atoi(const char *str, size_t n)
{
    spr_int_t value, temp;
    spr_int_t cutoff, cutlim;
    spr_uint_t i;

    if (n == 0) {
        return -1;
    }

    cutoff = SPR_INT_T_MAX / 10;
    cutlim = SPR_INT_T_MAX % 10;

    value = 0;

    for (i = 0; i < n; ++i) {
        if (str[i] < '0' || str[i] > '9') {
            return -1;
        }

        temp = str[i] - '0';

        if (value >= cutoff && (value > cutoff || temp > cutlim)) {
            return -1;
        }

        value = value * 10 + temp;
    }

    return value;
}

ssize_t
spr_atosz(const char *str, size_t n)
{
    ssize_t value, temp;
    ssize_t cutoff, cutlim;
    spr_uint_t i;

    if (n == 0) {
        return -1;
    }

    cutoff = SPR_SSIZE_T_MAX / 10;
    cutlim = SPR_SSIZE_T_MAX % 10;

    value = 0;

    for (i = 0; i < n; ++i) {
        if (str[i] < '0' || str[i] > '9') {
            return -1;
        }

        temp = str[i] - '0';

        if (value >= cutoff && (value > cutoff || temp > cutlim)) {
            return -1;
        }

        value = value * 10 + temp;
    }

    return value;
}

size_t
spr_strlen(const char *str)
{
    spr_uint_t i;

    for (i = 0; ; ++i) {

        if (str[i] == '\0') {
            return i;
        }
    }
}

size_t
spr_strnlen(const char *str, size_t n)
{
    spr_uint_t i;

    for (i = 0; i < n; ++i) {

        if (str[i] == '\0') {
            return i;
        }
    }

    return n;
}

spr_int_t
spr_strcasecmp(char *str1, char *str2)
{
    spr_uint_t c1, c2;

    for ( ; ; ) {
        c1 = (spr_uint_t) *str1++;
        c2 = (spr_uint_t) *str2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        if (c1 == c2) {

            if (c1) {
                continue;
            }

            return 0;
        }

        return c1 - c2;
    }
}

spr_int_t
spr_strncasecmp(char *str1, char *str2, size_t n)
{
    spr_uint_t c1, c2;

    while (n) {
        c1 = (spr_uint_t) *str1++;
        c2 = (spr_uint_t) *str2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        if (c1 == c2) {

            if (c1) {
                n--;
                continue;
            }

            return 0;
        }

        return c1 - c2;
    }

    return 0;
}

char *
spr_strchr(const char *str, int c)
{
    size_t len, i;

    len = spr_strlen(str);

    for (i = 0; i < len; ++i) {

        if (str[i] == c) {
            return (char *) &str[i];
        }
    }
    return NULL;
}

char *
spr_strrchr(const char *str, int c)
{
    size_t len;
    spr_int_t i;

    len = spr_strlen(str);

    for (i = len; i >= 0; i--) {

        if (str[i] == c) {
            return (char *) &str[i];
        }
    }
    return NULL;
}

char *
spr_pstrdup(spr_pool_t *pool, const char *str)
{
    char *mem;
    size_t len;

    len = spr_strlen(str) + 1;

    mem = spr_palloc(pool, len * sizeof(char));
    if (!mem) {
        return NULL;
    }

    spr_memmove(mem, str, len);

    return mem;
}

char *
spr_pstrndup(spr_pool_t *pool, const char *str, size_t n)
{
    char *mem;

    mem = spr_palloc(pool, (n + 1) * sizeof(char));
    if (!mem) {
        return NULL;
    }

    spr_memmove(mem, str, n);
    mem[n] = '\0';

    return mem;
}

char *
spr_pstrcat1(spr_pool_t *pool, ...)
{
    va_list args;
    char *str, *argv, *pos;
    size_t len;

    len = 0;

    va_start(args, pool);

    while ((argv = va_arg(args, char *)) != NULL) {
        len += spr_strlen(argv);
    }

    va_end(args);

    str = spr_palloc(pool, len + 1);
    if (!str) {
        return NULL;
    }

    pos = str;

    str[len] = '\0';

    va_start(args, pool);

    while ((argv = va_arg(args, char *)) != NULL) {
        len = spr_strlen(argv);
        spr_memmove(pos, argv, len);
        pos += len;
    }

    va_end(args);

    return str;
}
