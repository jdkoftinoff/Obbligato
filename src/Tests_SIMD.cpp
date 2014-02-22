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

typedef SIMD_Vector<float, 4> vec4float;
typedef SIMD_Vector<float, 8> vec8float;
typedef SIMD_Vector<double, 2> vec2double;
typedef SIMD_Vector<double, 4> vec4double;
typedef SIMD_Vector<vec4float, 16> audiochunk4channel;

template <typename T> T munger(T const &a, T const &b, T const &c, T const &d) {
    return a * b + b * c + c * d + d * sin(a) + sqrt(b);
}

template <typename T> bool test_init(T &r, int m) {
    int c = 0;

    for (size_t i = 0; i != simd_flattened_size<T>::value; ++i) {
        c += m;
        set_flattened_item(r, typename simd_flattened_type<T>::type(c), i);
    }
    return true;
}

template <typename V> void print(std::string label, V const &v) {
    ob_log_info( label_fmt(label) << v );
}

template <typename T> bool test_one_simd(T &r) {
    ob_log_info( label_fmt("Initializer") 
#ifndef _MSC_VER
       << __PRETTY_FUNCTION__ 
#endif
       );
    T a, b, c, d;
    test_init(a, 9);
    test_init(b, 2);
    test_init(c, 3);
    test_init(d, 4);
    print("a", a);
    print("b", b);
    print("c", c);
    print("d", d);
    r = (a + b + c + d);
    ob_log_info( label_fmt("a+b+c+d") << r );
    ob_log_info( label_fmt("sqrt(r)") << sqrt(r) );
    ob_log_info( label_fmt("cos(r)") << cos(r) );
    ob_log_info( label_fmt("arg(r)") << arg(r) );
    ob_log_info( label_fmt("abs(r)") << abs(r) );
    ob_log_info( label_fmt("reciprocal(r)") << reciprocal(r) );
    T ac = a;
    b = a + a;
    ob_log_info( label_fmt("b") << (b) );
    ob_log_info( label_fmt("ac") << (ac) );
    ob_log_info( label_fmt("ac+=b") << (ac += b) );
    ob_log_info( label_fmt("ac-=b") << (ac -= b) );
    ob_log_info( label_fmt("ac*=b") << (ac *= b) );
    ob_log_info( label_fmt("ac/=b") << (ac /= b) );

    return true;
}

bool test_simd() {

    double d = 99;
    test_one_simd(d);

    vec4float a4f;
    test_one_simd(a4f);

    vec8float a8f;
    test_one_simd(a8f);

    vec2double a2d;
    test_one_simd(a2d);

    vec4double a4d;
    test_one_simd(a4d);

    auto ref1 = make_simd_ref(a4d);
    ob_log_info( label_fmt("ref1") << ref1 );
    ob_log_info( is_simd_ref<decltype(ref1)>::value );
    ref1[0] = 9;
    ob_log_info( label_fmt("ref") << ref1 );

    vec4double const a4dc = a4d;
    auto ref2c = make_simd_ref(a4dc);
    ob_log_info( label_fmt("ref2c") << ref2c );
    ob_log_info( is_simd_ref<decltype(ref2c)>::value );

    auto ref3 = make_simd_ref(ref1);
    ob_log_info( label_fmt("ref3") << ref3 );

    auto ref4 = make_simd_ref(ref2c);
    ob_log_info( label_fmt("ref4 of ref2") << ref4 );

    vec4float a4f1 = a4f * 3.1415f;

    apply(a4f, [](float aa, float bb) { return aa + (bb * 2); }, a4f, a4f1);
    ob_log_info( label_fmt("a4f") << a4f );

    float m = munger(1.1f, 2.1f, 3.1f, 4.1f);
    ob_log_info( label_fmt("m") << m );

    vec4float m4 = munger(
        vec4float{1.1f, 1.2f, 1.3f, 1.4f}, vec4float{2.1f, 2.2f, 2.3f, 2.4f},
        vec4float{3.1f, 3.2f, 3.3f, 3.4f}, vec4float{4.1f, 4.2f, 4.3f, 4.4f});
    ob_log_info( label_fmt("m4") << m4 );

    vec4float m4a;
    vec4float m4a1 = {1.1f, 1.2f, 1.3f, 1.4f};
    vec4float m4a2 = {2.1f, 2.2f, 2.3f, 2.4f};
    vec4float m4a3 = {3.1f, 3.2f, 3.3f, 3.4f};
    vec4float m4a4 = {4.1f, 4.2f, 4.3f, 4.4f};

    apply(m4a, munger<float>, m4a1, m4a2, m4a3, m4a4);

    ob_log_info( label_fmt("m4a") << m4a );

    return false;
}
}
}
