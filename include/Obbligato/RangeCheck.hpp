#pragma once
#ifndef Obbligato_RangeCheck_hpp
#define Obbligato_RangeCheck_hpp

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

namespace Obbligato
{

    template< typename T >
    inline
    T find_maximum ( const T &a, const T &b )
    {
      return a > b ? a : b;
    }

    template< typename T >
    inline
    T find_maximum ( const T &a, const T &b, const T &c )
    {
      T ab = find_maximum ( a, b );
      return find_maximum ( ab, c );
    }

    template< typename T >
    inline
    T find_maximum ( const T &a, const T &b, const T &c, const T &d )
    {
      T abc = find_maximum ( a, b, c );
      return find_maximum ( abc, d );
    }

    template< typename T >
    inline
    T find_minimum ( const T &a, const T &b )
    {
      return a < b ? a : b;
    }

    template< typename T >
    inline
    T find_minimum ( const T &a, const T &b, const T &c )
    {
      T ab = find_minimum ( a, b );
      return find_minimum ( ab, c );
    }

    template< typename T >
    inline
    T find_minimum ( const T &a, const T &b, const T &c, const T &d )
    {
      T abc = find_minimum ( a, b, c );
      return find_minimum ( abc, d );
    }

    inline void range_check_error ( size_t val, size_t lower, size_t count,
                                         std::string file, int line )
    {
      std::stringstream s;
      s << "MSLI_RANGE_CHECK error in File: " << std::endl;
      s << file << ":" << line << std::endl;
      s << "value: " << val << std::endl;
      s << "lower limit: " << lower << std::endl;
      s << "upper limit: " << lower + count - 1 << std::endl;
      throw std::out_of_range ( s.str() );
    }


    /**
     * Validate buffer position in a buffer len for an element of elem_size.
     *
     * @param bufpos position of element in buffer, in octets
     * @param buflen length of buffer, in octets
     * @param elem_size element size, in octets
     * @return -1 if element does not fit, bufpos+elem_size if it does.
     */
    inline ssize_t buffer_range_check( ssize_t bufpos, size_t buflen, size_t elem_size )
    {
        return ((size_t)(bufpos) + (size_t)elem_size <= (size_t)buflen) ? (ssize_t)(bufpos + elem_size)  : (ssize_t)-1;
    }

    /*@}*/

}

#if defined(OBBLIGATO_ENABLE_RANGE_CHECK)
#define OBBLIGATO_RANGE_CHECK(val,lower,count) do { if( ! ( (val)>=(lower) && (val)<(lower+count) ) ) { ::Obbligato::msli_range_check_error(val,lower,count,::std::string(__FILE__),__LINE__); } } while(false)
#else
#define OBBLIGATO_RANGE_CHECK(val,lower,count) ((void)val)
#endif

#define OBBLIGATO_ALWAYS_RANGE_CHECK(val,lower,count) do { if( ! ( (val)>=(lower) && (val)<(lower+count) ) ) { ::Obbligato::msli_range_check_error(val,lower,count,::std::string(__FILE__),__LINE__); } } while(false)


#endif


