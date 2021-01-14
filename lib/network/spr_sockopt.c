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
#include "spr_sockopt.h"
#include "spr_errno.h"

#if (SPR_LINUX)

spr_err_t
spr_socket_nopush(spr_socket_t sockfd)
{
    int option;
    spr_err_t err;

    option = 1;

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
                        (const void *) &option, sizeof(int)) == -1)
    {
        err = spr_get_socket_errno();
        return err;
    }
    /*
     * int cork = 1;
     * return setsockopt(sockfd, IPPROTO_TCP, TCP_CORK,
     *                     (const void *) &cork, sizeof(int));
     */
    return SPR_OK;
}

spr_err_t
spr_socket_push(spr_socket_t sockfd)
{
    int option;
    spr_err_t err;

    option = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
                        (const void *) &option, sizeof(int)) == -1)
    {
        err = spr_get_socket_errno();
        return err;
    }
    return SPR_OK;
}

spr_err_t
spr_socket_nonblocking(spr_socket_t sockfd)
{
    int flags;
    spr_err_t err;

    flags = fcntl(sockfd, F_GETFL);

    if (fcntl(sockfd, F_SETFL, flags|O_NONBLOCK) == -1) {
        err = spr_get_socket_errno();
        return err;
    }
    return SPR_OK;
}

spr_err_t
spr_socket_blocking(spr_socket_t sockfd)
{
    int flags;
    spr_err_t err;

    flags = fcntl(sockfd, F_GETFL);

    if (fcntl(sockfd, F_SETFL, flags & (~O_NONBLOCK)) == -1) {
        err = spr_get_socket_errno();
        return err;
    }
    return SPR_OK;
}

#elif (SPR_FREEBSD)
spr_err_t
spr_socket_nopush(spr_socket_t sockfd)
{
    int option;
    spr_err_t err;

    option = 1;

    if (setsockopt(sockfd, IPPROTO_TCP, TCP_NOPUSH,
            (const void *) &option, sizeof(int)) == -1)
    {
        err = spr_get_socket_errno();
        return err;
    }
    return SPR_OK;
}

spr_err_t
spr_socket_push(spr_socket_t sockfd)
{
    int option;
    spr_err_t err;

    option = 0;

    if (setsockopt(sockfd, IPPROTO_TCP, TCP_NOPUSH,
            (const void *) &spr_socket_nopush, sizeof(int)) == -1)
    {
        err = spr_get_socket_errno();
        return err;
    }
    return SPR_OK;
}

spr_err_t
spr_socket_nonblocking(spr_socket_t sockfd)
{
    int nb;
    spr_err_t err;

    nb = 1;

    if (ioctl(sockfd, FIONBIO, &nb) == -1) {
        err = spr_get_socket_errno();
        return err;
    }
    return SPR_OK;
}

spr_err_t
spr_socket_blocking(spr_socket_t sockfd)
{
    int nb;
    spr_err_t err;

    nb = 0;

    if (ioctl(sockfd, FIONBIO, &nb) == -1) {
        err = spr_get_socket_errno();
        return err;
    }
    return SPR_OK;
}


#elif (SPR_DARWIN)


#elif (SPR_SOLARIS)
spr_err_t
spr_socket_nopush(spr_socket_t sockfd)
{
    /* XXX: do smth */
    (void) sockfd;
    return SPR_OK;
}

spr_err_t
spr_socket_push(spr_socket_t sockfd)
{
    /* XXX: do smth */
    (void) sockfd;
    return SPR_OK;
}

/* 
 * Setting FIONBIO or O_NDELAY on Solaris causes a read
 * with no data to return 0 on a tty or pipe, or -1 with
 * errno EAGAIN on a socket. However 0 is ambiguous since
 * it is also returned for EOF
 */
spr_err_t
spr_socket_nonblocking(spr_socket_t sockfd)
{
    int nb;
    spr_err_t err;

    nb = 1;

    if (ioctl(sockfd, FIONBIO, &nb) == -1) {
        err = spr_get_socket_errno();
        return err;
    }
    return SPR_OK;
}

spr_err_t
spr_socket_blocking(spr_socket_t sockfd)
{
    int nb;
    spr_err_t err;

    nb = 0;

    if (ioctl(sockfd, FIONBIO, &nb) == -1) {
        err = spr_get_socket_errno();
        return err;
    }
    return SPR_OK;
}


#elif (SPR_WIN32)

spr_err_t
spr_socket_nopush(spr_socket_t sockfd)
{
    /* XXX: do smth */
    (void) sockfd;
    return SPR_OK;
}

spr_err_t
spr_socket_push(spr_socket_t sockfd)
{
    /* XXX: do smth */
    (void) sockfd;
    return SPR_OK;
}

spr_err_t
spr_socket_nonblocking(spr_socket_t sockfd)
{
    DWORD nb;
    spr_err_t err;

    nb = 1;

    if (ioctlsocket(sockfd, FIONBIO, &nb) != 0) {
        err = spr_get_socket_errno();
        return err;
    }
    return SPR_OK;
}

spr_err_t
spr_socket_blocking(spr_socket_t sockfd)
{
    DWORD nb;
    spr_err_t err;

    nb = 0;

    if (ioctlsocket(sockfd, FIONBIO, &nb) != 0) {
        err = spr_get_socket_errno();
        return err;
    }
    return SPR_OK;
}

#endif
