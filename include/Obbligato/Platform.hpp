#pragma once
#ifndef Obbligato_Platform_hpp
#define Obbligato_Platform_hpp

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

#include <stdexcept>
#include <string>
#include <string.h>

#if defined(__clang__)

# include "Obbligato/Platform_Compiler_Clang.hpp"

#elif defined(__GNUC__)

# include "Obbligato/Platform_Compiler_GCC.hpp"

#elif defined(_MSC_VER)

# include "Obbligato/Platform_Compiler_MSVC.hpp"

#endif


#ifdef __linux__
# include "Obbligato/Platform_Posix.hpp"
# include "Obbligato/Platform_Linux.hpp"
#endif

#if defined(__APPLE__) && (TARGET_OS_IPHONE==0)
# include "Obbligato/Platform_Posix.hpp"
# include "Obbligato/Platform_MacOSX.hpp"
#endif

#if defined(__APPLE__) && (TARGET_OS_IPHONE==1)
# include "Obbligato/Platform_Posix.hpp"
# include "Obbligato/Platform_IOS.hpp"
#endif

#ifdef WIN32
# include "Obbligato/Platform_Win32.hpp"
#endif

#ifndef OBBLIGATO_PLATFORM_HAS_TR1_MEMORY
# define OBBLIGATO_PLATFORM_HAS_TR1_MEMORY (0)
#endif

#ifndef OBBLIGATO_PLATFORM_HAS_CXX11_MEMORY
# define OBBLIGATO_PLATFORM_HAS_CXX11_MEMORY (0)
#endif

#ifndef OBBLIGATO_PLATFORM_HAS_TR1_FUNCTIONAL
# define OBBLIGATO_PLATFORM_HAS_TR1_FUNCTIONAL (0)
#endif

#ifndef OBBLIGATO_PLATFORM_HAS_CXX11_FUNCTIONAL
# define OBBLIGATO_PLATFORM_HAS_CXX11_FUNCTIONAL (0)
#endif

#ifndef OBBLIGATO_PLATFORM_HAS_RVALUE_REFS
# define OBBLIGATO_PLATFORM_HAS_RVALUE_REFS (0)
#endif

#ifndef OBBLIGATO_PLATFORM_HAS_VARIADIC_TMPL
# define OBBLIGATO_PLATFORM_HAS_VARIADIC_TMPL (0)
#endif

#ifndef OBBLIGATO_PLATFORM_VECTOR_ALIGN
# define OBBLIGATO_PLATFORM_VECTOR_ALIGN
#endif

#ifndef OBBLIGATO_PLATFORM_VECTOR_ALIGN_
# define OBBLIGATO_PLATFORM_VECTOR_ALIGN_
#endif

namespace Obbligato {

inline void throw_errno( int e )
{
#ifdef WIN32
    char ebuf[1024];
    strerror_s(ebuf,e);
    throw std::runtime_error("errno: " + std::string(ebuf) );
#else
    throw std::runtime_error("errno: " + std::string(strerror(e)));
#endif
}

inline void throw_gai_error( int e )
{
#ifdef WIN32
    const char *s = gai_strerrorA(e);
#else
    const char *s = gai_strerror(e);
#endif
    throw std::runtime_error(std::string("gaierror: ") + std::string(s) );
}

}

#endif




