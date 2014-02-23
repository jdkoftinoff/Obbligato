#pragma once
/*
 Copyright (c) 2013, J.D. Koftinoff Software, Ltd. <jeffk@jdkoftinoff.com>
 http://www.jdkoftinoff.com/
 All rights reserved.

 Permission to use, copy, modify, and/or distribute this software for any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.

 THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifdef _WIN32
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
#endif

#define WIN32_LEAN_AND_MEAN

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT NTDDI_VISTASP1
#endif
#ifndef WINVER
#define WINVER _WIN32_WINNT
#endif

#include <SDKDDKVer.h>

#ifdef _MSC_VER
#pragma warning(disable : 4267)
#pragma warning(disable : 4345)

#pragma comment(lib, "Ws2_32.lib")
#endif

#include <WinSock2.h>
#include <windows.h>
#include <WS2ipdef.h>
#include <WS2tcpip.h>

#ifdef _MSC_VER
#include <Iphlpapi.h>
typedef SSIZE_T ssize_t;
#define noexcept throw()
#endif

typedef SOCKET socket_fd_t;

inline void sleep(int sec) { Sleep(sec * 1000); }

inline void usleep(int usec) { Sleep(usec / 1000); }

namespace Obbligato {
namespace Platform {}
}

#endif
