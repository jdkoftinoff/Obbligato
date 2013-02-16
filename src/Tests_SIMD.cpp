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
#include "Obbligato/Tests_SIMD.hpp"
#include "Obbligato/SIMD.hpp"
#include "Obbligato/IOStream.hpp"
#include "Obbligato/Test.hpp"

namespace Obbligato
{
    namespace Tests
    {
        using namespace Obbligato::SIMD;
        using namespace Obbligato::IOStream;
        using namespace Obbligato::Test;
        
        typedef SIMD_Vector<float, 4> vector_small_t;
        
        typedef SIMD_Vector< vector_small_t, 128 > vector_big_t;
        
        vector_big_t big_a, big_b, big_c, big_d;
        
        template <typename T>
        T munger( T const &a, T const &b, T const &c, T const &d )
        {
            return a*b + b*c + c*d + d*a;
        }
        
        template <typename T, size_t N>
        void fill( SIMD_Vector<T, N> &v )
        {
            for( size_t i=0; i<N; ++i )
            {
                set( v, T(i+1), i );
            }
        }
        
        
        
        bool test_simd()
        {
            vector_small_t a,b,c,d;
            
            fill(a);
            
            b=vector_small_t(3);
            c=vector_small_t(99);
            
            for( size_t i=0; i<vector_big_t::vector_size; ++i )
            {
                set(big_b,a,i);
                set(big_c,b,i);
                a+=b;
                set(big_d,c,i);
                c+=b;
            }
            
            big_a.apply( munger<vector_small_t>, big_a, big_b, big_c, big_d );
            
            ob_cinfo << fmt( title("Vector dump" ) );
            ob_cinfo << fmt( label("a") ) << a << std::endl;
            ob_cinfo << b << std::endl;
            ob_cinfo << get(b,0) << std::endl;
            ob_cinfo << get(b,1) << std::endl;
            ob_cinfo << big_a << std::endl;
            
            return false;
        }
    }
}



