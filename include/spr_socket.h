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

#ifndef INCLUDED_SPR_SOCKET_H
#define INCLUDED_SPR_SOCKET_H

#include "spr_portable.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (SPR_LINUX)
#define SPR_SHUT_RD                  SHUT_RD
#define SPR_SHUT_WR                  SHUT_WR
#define SPR_SHUT_RDWR                SHUT_RDWR
#define SPR_INVALID_SOCKET           -1

#define SPR_FMT_SOCKET_T             "%d"

#elif (SPR_FREEBSD)
#define SPR_SHUT_RD                  SHUT_RD
#define SPR_SHUT_WR                  SHUT_WR
#define SPR_SHUT_RDWR                SHUT_RDWR
#define SPR_INVALID_SOCKET           -1

#define SPR_FMT_SOCKET_T             "%d"

#elif (SPR_SOLARIS)
/* Details for Solaris */
#define SPR_INVALID_SOCKET           -1

#elif (SPR_DARWIN)
/* Details for Darwin */
#define SPR_INVALID_SOCKET           -1

#elif (SPR_WIN32)
#define SPR_SHUT_RD                  SD_RECEIVE
#define SPR_SHUT_WR                  SD_SEND
#define SPR_SHUT_RDWR                SD_BOTH
#define SPR_INVALID_SOCKET           INVALID_SOCKET

#if (SPR_PTR_SIZE == 4)
#define SPR_FMT_SOCKET_T             "%d"
#else
#define SPR_FMT_SOCKET_T             "%I64d"
#endif

spr_err_t spr_winsock_init_v22(void);

#endif

spr_err_t spr_socket_init(spr_socket_t *sockfd, int domain, int type,
    int protocol);
spr_err_t spr_socket_bind(spr_socket_t sockfd,
    const struct sockaddr *addr, spr_socklen_t addrlen);
spr_err_t spr_socket_listen(spr_socket_t sockfd, int backlog);
spr_err_t spr_socket_accept(spr_socket_t *new_sockfd,
    spr_socket_t sockfd, struct sockaddr *addr, spr_socklen_t *addrlen);
spr_err_t spr_socket_connect(spr_socket_t sockfd,
    const struct sockaddr *addr, spr_socklen_t addrlen);
spr_err_t spr_socket_shutdown(spr_socket_t sockfd, int how);
void spr_socket_close(spr_socket_t sockf);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_SPR_SOCKET_H */
