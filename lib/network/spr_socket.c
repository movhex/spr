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
#include "spr_socket.h"
#include "spr_errno.h"

spr_err_t
spr_socket_init(spr_socket_t *new_sockfd, int domain, int type,
    int protocol)
{
    spr_socket_t sockfd;
    spr_err_t err;

    sockfd = socket(domain, type, protocol);
    if (sockfd == SPR_INVALID_SOCKET) {
        err = spr_get_socket_errno();
        goto failed;
    }

    *new_sockfd = sockfd;

    return SPR_OK;

failed:
    return err;
}

spr_err_t
spr_socket_bind(spr_socket_t sockfd, const struct sockaddr *addr,
    spr_socklen_t addrlen)
{
    if (bind(sockfd, addr, addrlen) == -1) {
        return spr_get_socket_errno();
    }

    return SPR_OK;
}

spr_err_t
spr_socket_listen(spr_socket_t sockfd, int backlog)
{
    if (listen(sockfd, backlog) == -1) {
        return spr_get_socket_errno();
    }

    return SPR_OK;
}

spr_err_t
spr_socket_accept(spr_socket_t *new_sockfd, spr_socket_t sockfd,
    struct sockaddr *addr, spr_socklen_t *addrlen)
{
    spr_socket_t sockfd1;

    sockfd1 = accept(sockfd, addr, addrlen);
    if (sockfd1 == SPR_INVALID_SOCKET) {
        return spr_get_socket_errno();
    }

    *new_sockfd = sockfd1;

    return SPR_OK;
}

spr_err_t
spr_socket_connect(spr_socket_t sockfd, const struct sockaddr *addr,
    spr_socklen_t addrlen)
{
    if (connect(sockfd, addr, addrlen) == -1) {
        return spr_get_socket_errno();
    }

    return SPR_OK;
}

spr_err_t
spr_socket_shutdown(spr_socket_t sockfd, int how)
{
    if (shutdown(sockfd, how) == -1) {
        return spr_get_socket_errno();
    }

    return SPR_OK;
}

#if (SPR_POSIX)

void
spr_socket_close(spr_socket_t sockfd)
{
    close(sockfd);
}

#elif (SPR_WIN32)

void
spr_socket_close(spr_socket_t sockfd)
{
    closesocket(sockfd);
}

spr_err_t
spr_winsock_init_v22(void)
{
    WSADATA data;
    WORD version_requested;

    version_requested = MAKEWORD(2, 2);

    if (WSAStartup(version_requested, &data) != 0) {
        return spr_get_socket_errno();
    }
    return SPR_OK;
}

#endif
