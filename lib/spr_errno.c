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

#include "spr_errno.h"
#include "spr_memory.h"
#include "spr_string.h"

#define SPR_ERR_UNKNOW  2147483647

static const struct {
    spr_err_t code;
    const char *message;
} error_list[] = {
    {SPR_OK,                       "OK"},
    {SPR_FAILED,                   "Internal error"},
    {SPR_BUSY,                     "Resource is busy"},
    {SPR_DONE,                     "Operation is done"},
    {SPR_ABORT,                    "Operation aborted"},
    {SPR_DECLINED,                 "Operation declined"},
    {SPR_NOT_FOUND,                "Resource not found"},

    /* SPR_ERR_FILESYS domain */
    {SPR_ERR_FILESYS_ABS_PATH,     "Specified path is not absolute"},
    {SPR_ERR_FILESYS_LONG_PATH,    "Result path is too long"},

    /* End of error list */
    {SPR_ERR_UNKNOW,               "Unknown error code"}
};


#if (SPR_POSIX)

static const char *
spr_os_strerror(spr_err_t err, char *buf, size_t bufsize)
{
    char *str;
    size_t len;

    str = strerror(err);
    len = spr_strlen(str);

    if (len < bufsize) {
        spr_memmove(buf, str, len);
        buf[len] = '\0';
    }
    else {
        spr_memmove(buf, str, bufsize);
        buf[bufsize-1] = '\0';
    }
    return buf;
}


#elif (SPR_WIN32)

static const char *
spr_os_strerror(spr_err_t err, char *buf, size_t bufsize)
{
    DWORD lang, len;

    lang = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);

    if (bufsize == 0) {
        return NULL;
    }

    len = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                        NULL, err, lang, buf, bufsize, NULL);

    if (len == 0 && lang) {
        lang = 0;
        len = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, lang,
                                                (char *) buf, bufsize, NULL);
    }

    if (len == 0) {
        return NULL;
    }

    /* remove ".\r\n\0" */
    while (buf[len] == '\0' || buf[len] == '\r'
                            || buf[len] == '\n' || buf[len] == '.')
    {
        buf[len] = '\0';
        len -= 1;
    }
    return buf;
}
#endif

const char *
spr_strerror(spr_err_t err, char *buf, size_t bufsize)
{
    const char *str;
    size_t len;
    spr_uint_t i;

    if (err > 0 && err < SPR_ERROR_DOMAIN_BASE) {
        str = spr_os_strerror(err, buf, bufsize);
        if (!str) {
            return spr_strerror(SPR_ERR_UNKNOW, buf, bufsize);
        }

        return str;
    }

    for (i = 0; ; ++i) {
        if (error_list[i].code == err ||
            error_list[i].code == SPR_ERR_UNKNOW)
        {
            str = error_list[i].message;
            len = spr_strlen(str);
            if (len < bufsize) {
                spr_memmove(buf, str, len);
                buf[len] = '\0';
            }
            else {
                spr_memmove(buf, str, bufsize);
                buf[bufsize-1] = '\0';
            }
            break;
        }
        else {
            continue;
        }
    }

    return buf;
}
