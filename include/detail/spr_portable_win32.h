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

#if !defined(SPR_INCLUDE_INTERNAL_HEADERS)
#if defined(_MSC_VER)
#pragma message("spr_portable_win32.h is an internal header file and must \
not be used directly. Please use spr_portable.h instead.")
#else
#warning "spr_portable_win32.h is an internal header file and must not be \
used directly. Please use spr_portable.h instead."
#endif
#define SPR_INCLUDE_INTERNAL_HEADERS
#endif

#ifndef INCLUDED_SPR_PORTABLE_WIN32_H
#define INCLUDED_SPR_PORTABLE_WIN32_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>
#include <ws2ipdef.h>
#include <windows.h>
#include <process.h>
#include <io.h>

typedef HANDLE                         spr_fd_t;
typedef BY_HANDLE_FILE_INFORMATION     spr_file_stat_t;
typedef SOCKET                         spr_socket_t;
typedef socklen_t                      spr_socklen_t;
typedef DWORD                          spr_tid_t;
typedef HANDLE                         spr_thread_handle_t;
typedef DWORD                          spr_thread_value_t;
typedef SYSTEMTIME                     spr_tm_t;

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_SPR_PORTABLE_WIN32_H */
