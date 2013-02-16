#pragma once
#ifndef Obbligato_StaticAssert_hpp
#define Obbligato_StaticAssert_hpp

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

#include "Obbligato/World.hpp"

#define OB_STATIC_ASSERT( a ) typedef typename ::Obbligato::StaticAssertFailure< bool(a) >::success OBBLIGATO_JOIN2( _obbligato_static_assert_, __LINE__ )

#define OB_STATIC_ASSERT_POWER_OF_TWO( a ) OB_STATIC_ASSERT( ((a) & ((a)-1)) == 0 )


namespace Obbligato
{
    template < bool > struct StaticAssertFailure;

    template <> struct StaticAssertFailure<true>
    {
        typedef void success;
    };
}


#endif
