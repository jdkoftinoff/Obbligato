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
#include "Obbligato/Traits.hpp"

#if __cplusplus >= 201103L

namespace Obbligato
{
namespace SIMD
{
}
}

#include "Obbligato/SIMD_Vector.hpp"

#if defined( __ARM_NEON__ )
#include "Obbligato/SIMD_VectorNEON32x4.hpp"
#endif

#if defined( __SSE2__ )
#include "Obbligato/SIMD_VectorSSE32x4.hpp"
#include "Obbligato/SIMD_VectorSSE64x2.hpp"
#endif

#if defined( __AVX__ )
#include "Obbligato/SIMD_VectorAVX32x8.hpp"
#include "Obbligato/SIMD_VectorAVX64x4.hpp"
#endif
#endif
