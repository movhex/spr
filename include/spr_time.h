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

#ifndef INCLUDED_SPR_TIME_H
#define INCLUDED_SPR_TIME_H

#include "spr_portable.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (SPR_POSIX)

#define spr_tm_sec                   tm_sec
#define spr_tm_min                   tm_min
#define spr_tm_hour                  tm_hour
#define spr_tm_mday                  tm_mday
#define spr_tm_mon                   tm_mon
#define spr_tm_year                  tm_year
#define spr_tm_wday                  tm_wday

#define spr_tm_sec_t                 int
#define spr_tm_min_t                 int
#define spr_tm_hour_t                int
#define spr_tm_mday_t                int
#define spr_tm_mon_t                 int
#define spr_tm_year_t                int
#define spr_tm_wday_t                int

#define spr_msleep(ms)               usleep(ms * 1000)
#define spr_pause()                  pause()


#elif (SPR_WIN32)

#define spr_tm_sec                   wSecond
#define spr_tm_min                   wMinute
#define spr_tm_hour                  wHour
#define spr_tm_mday                  wDay
#define spr_tm_mon                   wMonth
#define spr_tm_year                  wYear
#define spr_tm_wday                  wDayOfWeek

#define spr_tm_sec_t                 WORD
#define spr_tm_min_t                 WORD
#define spr_tm_hour_t                WORD
#define spr_tm_mday_t                WORD
#define spr_tm_mon_t                 WORD
#define spr_tm_year_t                WORD
#define spr_tm_wday_t                WORD

#define spr_msleep(ms)               Sleep(ms)

#endif

void spr_gettimeofday(struct timeval *tv);
void spr_localtime(time_t sec, spr_tm_t *tm);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_SPR_TIME_H */
