#pragma once
/*
 Copyright (c) 2013, J.D. Koftinoff Software, Ltd.
 <jeffk@jdkoftinoff.com>
 http://www.jdkoftinoff.com/
 All rights reserved.

 Permission to use, copy, modify, and/or distribute this software for
 any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.

 THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 WARRANTIES
 WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifdef _WIN32

#undef NTDDI_VERSION
#define NTDDI_VERSION 0x06010000

#undef WINVER
#define WINVER 0x601

#undef _WIN32_WINNT
#define _WIN32_WINNT 0x601

#ifndef BYTE_ORDER
#define BYTE_ORDER 1234
#endif
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 1234
#endif
#ifndef BIG_ENDIAN
#define BIG_ENDIAN 4321
#endif

#ifdef _MSC_VER
#include <WinSDKVer.h>

#include <SDKDDKVer.h>

#include <WinSock2.h>
#include <windows.h>
#include <WS2ipdef.h>
#include <WS2tcpip.h>

#include <Iphlpapi.h>



#if _MSC_VER < 1800
#ifndef ssize_t
#define ssize_t SSIZE_T
#endif
#else
# ifdef _WIN64
typedef __int64 ssize_t;
# else
typedef __int32 ssize_t;
# endif
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#endif

#else

#include <WinSock2.h>
#include <windows.h>
#include <WS2ipdef.h>
#include <WS2tcpip.h>

#include <Iphlpapi.h>
#ifndef AF_LINK
/// MINGW for WIN32 is missing AF_LINK and sockaddr_dl for WINVER >=
/// 0x601
#undef AF_MAX
#define AF_LINK 33
#define AF_MAX 34
typedef struct sockaddr_dl
{
    ADDRESS_FAMILY sdl_family;
    UCHAR sdl_data[8];
    UCHAR sdl_zero[4];
} SOCKADDR_DL, *PSOCKADDR_DL;
#endif

#ifndef AI_NUMERICSERV
#define AI_NUMERICSERV 0x00000008
#endif

#endif

typedef SOCKET socket_fd_t;

inline void sleep( int sec ) { Sleep( sec * 1000 ); }

inline void usleep( int usec ) { Sleep( usec / 1000 ); }

namespace Obbligato
{
namespace Platform
{
}
}
#endif
