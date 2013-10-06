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

#include "Obbligato/World.hpp"
#include "Obbligato/SIMD.hpp"
#include "Obbligato/IOStream.hpp"
#include "Obbligato/Form.hpp"

namespace Obbligato {
namespace DSP {

using namespace SIMD;

template <typename PluginType, typename T, size_t PluginCount>
struct PluginChain {
    typedef PluginType plugin_type;
    typedef T value_type;
    enum {
        plugin_count = PluginCount
    };
    
    plugin_type item[plugin_count];
    
    plugin_type & operator[] ( size_t i ) {
        return item[i];
    }
    
    plugin_type const & operator [] ( size_t i ) const {
        return item[i];
    }

    size_t size() const {
        return plugin_count;
    }

    template < typename U, size_t M >
    SIMD_Vector<U,M> operator()( SIMD_Vector<U,M> const &input_value ) {
        SIMD_Vector<U,M> r = input_value;
        for( size_t i=0; i<plugin_count; ++i ) {
            for( size_t s=0; s<M; ++s ) {
                r[s] = item[i]( r[s] );
            }
        }
        return r;
    }
    
    T operator()( T input_value ) {
        T r = input_value;
        for( size_t i=0; i<plugin_count; ++i ) {
            r = item[i]( r );
        }
        return r;
    }
    
    friend std::ostream & operator << ( std::ostream &o, PluginChain const &v ) {
        using namespace IOStream;
        for( size_t i=0; i<v.size(); ++i ) {
            o << title_fmt(form<128>("item[%d]",i)) << std::endl;
            o << v.item[i] << std::endl;
        }
        return o;
    }
    
};



}
}
