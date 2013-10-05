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

namespace Obbligato {
namespace Tests {

using namespace Obbligato::SIMD;
using namespace Obbligato::IOStream;
using namespace Obbligato::Test;

typedef SIMD_Vector< std::complex<float>, 4> vec4complex;
typedef SIMD_Vector<float, 4> vec4float;
typedef SIMD_Vector<float, 8> vec8float;
typedef SIMD_Vector<double, 2> vec2double;
typedef SIMD_Vector<double, 4> vec4double;
typedef SIMD_Vector<vec4float, 16> audiochunk4channel;


template <typename T> T munger(T const &a, T const &b, T const &c, T const &d) {
    return a * b + b * c + c * d + d * a;
}

template <typename T>
bool test_init( T &r, int m ) {
    T a;
    int c=0;
    for( auto i=std::begin(a); i!=std::end(a); ++i, ++c ) {
        simd_splat(*i,(float)c);
    }
    r=a;
    return true;
}

template <typename T>
bool test_init( SIMD_Vector<T,2> &r, int m ) {
    SIMD_Vector<T,2> a {1*m,2*m};
    r=a;
    return true;
}

template <typename T>
bool test_init( SIMD_Vector<T,4> &r, int m ) {
    SIMD_Vector<T,4> a {1*m,2*m,3*m,4*m};
    r=a;
    return true;
}

template <typename T>
bool test_init( SIMD_Vector<T,8> &r, int m ) {
    SIMD_Vector<T,8> a {1*m,2*m,3*m,4*m,5*m,6*m,7*m,8*m};
    r=a;
    return true;
}

template <typename V>
void print( std::string label, V const &v ) {
    ob_cinfo << label_fmt(label) <<  v << std::endl;
}


template <typename V>
bool test_one_simd( V &r ) {
    ob_cinfo << title_fmt("Initializer") << std::endl;
    V a,b,c,d;
    test_init(a,9);
    test_init(b,2);
    test_init(c,3);
    test_init(d,4);
    ob_cinfo << title_fmt("one") << simd_one(a) << std::endl;
    print( "a", a );
    print( "b", b );
    print( "c", c );
    print( "d", d );
    r=(a+b+c+d);
    ob_cinfo << title_fmt("a+b+c+d") << r << std::endl;
    ob_cinfo << title_fmt("sqrt(r)") << simd_sqrt(r) << std::endl;
    ob_cinfo << title_fmt("cos(r)") << simd_cos(r) << std::endl;
    ob_cinfo << title_fmt("arg(r)") << simd_arg(r) << std::endl;
    ob_cinfo << title_fmt("abs(r)") << simd_abs(r) << std::endl;
    V ac = a;
    b = a+a;
    ob_cinfo << title_fmt("b") << (b) << std::endl;
    ob_cinfo << title_fmt("ac") << (ac) << std::endl;
    ob_cinfo << title_fmt("ac+=b") << (ac+=b) << std::endl;
    ob_cinfo << title_fmt("ac-=b") << (ac-=b) << std::endl;
    ob_cinfo << title_fmt("ac*=b") << (ac*=b) << std::endl;
    ob_cinfo << title_fmt("ac/=b") << (ac/=b) << std::endl;
    
    return true;
}


bool test_simd() {

    vec4float a4f;
    test_one_simd(a4f);
    
    vec8float a8f;
    test_one_simd(a8f);

    vec2double a2d;
    test_one_simd(a2d);

    vec4double a4d;
    test_one_simd(a4d);
    
    vec4complex a4c;
    test_one_simd(a4c);

    audiochunk4channel ac4c;
    test_one_simd(ac4c);

    ac4c[1][0] = 99999;
    std::cout << ac4c << std::endl;

    std::array<float,7> ax;
    test_one_simd(ax);

    auto ref = make_simd_ref(a4d);
    std::cout << "ref: " << ref << std::endl;
#if 0
    apply(
        a,
        [](float aa, float bb) { return aa + (bb * 2); },
        b, c
        );
    
    std::cout << a << std::endl;
    
    vector_small_t e;

    a += one(e);

    std::cout << a << std::endl;
    
    std::cout << sin(a) << std::endl;
    
    std::cout << get_item(a,2) << std::endl;
    
    for (size_t i = 0; i < vector_big_t::vector_size; ++i) {
        big_b[i] = a;
        big_c[i] = b;
        a += b;
        big_d[i] = c;
        c += b;
    }
    
    big_a.apply(munger<vector_small_t>, big_a, big_b, big_c, big_d);

    ob_cinfo << title_fmt("Vector dump");
    ob_cinfo << label_fmt("a") << a << std::endl;
    ob_cinfo << b << std::endl;
    ob_cinfo << get_item(b, 0) << std::endl;
    ob_cinfo << get_item(b, 1) << std::endl;
    ob_cinfo << big_a << std::endl;
#endif

    return false;
}
}
}
