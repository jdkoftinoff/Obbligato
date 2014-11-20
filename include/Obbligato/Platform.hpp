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

#if defined( __clang__ )

#include "Obbligato/Platform_Compiler_Clang.hpp"

#elif defined( __GNUC__ )

#include "Obbligato/Platform_Compiler_GCC.hpp"

#elif defined( _MSC_VER )

#include "Obbligato/Platform_Compiler_MSVC.hpp"

#endif

#ifdef __linux__
#include "Obbligato/Platform_Posix.hpp"
#include "Obbligato/Platform_Linux.hpp"
#endif

#if defined(__APPLE__)
#include "TargetConditionals.h"
#include <CoreFoundation/CoreFoundation.h>
#endif

#if defined( __APPLE__ ) && ( TARGET_OS_IPHONE == 0 )
#include "Obbligato/Platform_Posix.hpp"
#include "Obbligato/Platform_MacOSX.hpp"
#endif

#if defined( __APPLE__ ) && ( TARGET_OS_IPHONE == 1 )
#include "Obbligato/Platform_Posix.hpp"
#include "Obbligato/Platform_IOS.hpp"
#endif

#ifdef _WIN32
#include "Obbligato/Platform_Win32.hpp"
#endif

#ifndef OBBLIGATO_PLATFORM_HAS_TR1_MEMORY
#define OBBLIGATO_PLATFORM_HAS_TR1_MEMORY ( 0 )
#endif

#ifndef OBBLIGATO_PLATFORM_HAS_CXX11_MEMORY
#define OBBLIGATO_PLATFORM_HAS_CXX11_MEMORY ( 0 )
#endif

#ifndef OBBLIGATO_PLATFORM_HAS_TR1_FUNCTIONAL
#define OBBLIGATO_PLATFORM_HAS_TR1_FUNCTIONAL ( 0 )
#endif

#ifndef OBBLIGATO_PLATFORM_HAS_CXX11_FUNCTIONAL
#define OBBLIGATO_PLATFORM_HAS_CXX11_FUNCTIONAL ( 0 )
#endif

#ifndef OBBLIGATO_PLATFORM_HAS_RVALUE_REFS
#define OBBLIGATO_PLATFORM_HAS_RVALUE_REFS ( 0 )
#endif

#ifndef OBBLIGATO_PLATFORM_HAS_VARIADIC_TMPL
#define OBBLIGATO_PLATFORM_HAS_VARIADIC_TMPL ( 0 )
#endif

#ifndef OBBLIGATO_PLATFORM_VECTOR_ALIGN
#define OBBLIGATO_PLATFORM_VECTOR_ALIGN
#endif

#ifndef OBBLIGATO_PLATFORM_VECTOR_ALIGN_
#define OBBLIGATO_PLATFORM_VECTOR_ALIGN_
#endif

namespace Obbligato
{
}
