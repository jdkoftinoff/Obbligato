#pragma once
#ifndef Obbligato_Platform_Compiler_GCC_hpp
#define Obbligato_Platform_Compiler_GCC_hpp

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


#if defined(__GNUC__) && !defined(__clang__)

# if _GCC_VER < 40601
#  define nullptr NULL
# endif

# if _GCC_VER > 40200
#  define OBBLIGATO_PLATFORM_HAS_TR1_MEMORY (0)
# define OBBLIGATO_PLATFORM_HAS_TR1_FUNCTIONAL (0)
# else
#  define OBBLIGATO_PLATFORM_HAS_TR1_MEMORY (1)
# define OBBLIGATO_PLATFORM_HAS_TR1_FUNCTIONAL (1)
# endif


#define OBBLIGATO_PLATFORM_VECTOR_ALIGN __attribute__ ((aligned (16)))

#endif

#endif


