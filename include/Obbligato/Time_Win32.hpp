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

#include "Obbligato/World.hpp"

#if defined( _WIN32 )

namespace Obbligato
{
#if WINVER >= 0x0600
typedef uint64_t Timestamp;
#else
typedef uint32_t Timestamp;
#endif
}

namespace Obbligato
{
namespace Time
{

static inline Timestamp get_processor_timestamp()
{
    LARGE_INTEGER v;
    v.QuadPart = 0;
    QueryPerformanceCounter( &v );
    return v.QuadPart;
}

static inline Timestamp get_current_timestamp()
{
#if WINNT >= 0x0600
    return GetTickCount64();
#else
    return GetTickCount();
#endif
}
}
}

#endif
