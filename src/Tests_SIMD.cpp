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
#include "Obbligato/DSP.hpp"

namespace Obbligato {
namespace Tests {

using namespace Obbligato::SIMD;
using namespace Obbligato::IOStream;
using namespace Obbligato::Test;
using namespace Obbligato::DSP;

typedef SIMD_Vector<std::complex<float>, 4> vec4complex;
typedef SIMD_Vector<float, 4> vec4float;
typedef SIMD_Vector<float, 8> vec8float;
typedef SIMD_Vector<double, 2> vec2double;
typedef SIMD_Vector<double, 4> vec4double;
typedef SIMD_Vector<vec4float, 16> audiochunk4channel;

template <typename T, size_t N> bool test_simd_biquad_one() {
    SIMD_Vector<T, N> input_audio, output_audio;

    PluginChain<Biquad<T>, T, 2> chain;

    for (size_t i = 0; i < simd_size<T>::value; ++i) {
        chain[0].coeffs.calculate_lowpass(i, 96000.0, 10000.0 * (i + 1), 1.0);
    }

    for (size_t i = 0; i < simd_size<T>::value; ++i) {
        chain[1]
            .coeffs.calculate_peak(i, 96000.0, 10000.0 * (i + 1), 0.707, 10.0);
    }

    ob_cinfo << title_fmt("plugin_chain") << std::endl << chain << std::endl;

    // create impulse on input audio
    zero(input_audio);
    one(input_audio[0]);

    ob_cinfo << label_fmt("input_audio") << input_audio << std::endl;

    // process the biquads in series on a per sample basis
    for (size_t i = 0; i < N; ++i) {
        output_audio[i] = chain(input_audio[i]);
    }

    ob_cinfo << label_fmt("biquad output") << output_audio << std::endl;

    // do more processing with the biquads in series on a per chunk basis
    zero(input_audio);

    output_audio = chain(input_audio);

    ob_cinfo << label_fmt("biquad output") << output_audio << std::endl;

    return true;
}

bool test_simd_biquad() {
    ob_cinfo << title_fmt("biquad float") << std::endl;
    test_simd_biquad_one<float, 8>();
    ob_cinfo << title_fmt("biquad float x 4") << std::endl;
    test_simd_biquad_one<SIMD_Vector<float, 4>, 8>();
    ob_cinfo << title_fmt("biquad double") << std::endl;
    test_simd_biquad_one<double, 8>();
    ob_cinfo << title_fmt("biquad double x 2") << std::endl;
    test_simd_biquad_one<SIMD_Vector<double, 2>, 8>();
    return true;
}

template <typename T, size_t N> bool test_simd_oscillator_one() {
    SIMD_Vector<T, N> input_audio, output_audio;
    zero(input_audio);
    zero(output_audio);

    PluginChain<Oscillator<T>, T, 2> chain;

    for (size_t i = 0; i < simd_size<T>::value; ++i) {
        chain[0].coeffs.set_amplitude(1.0, i);
        chain[0].state.set_frequency(96000.0, 5000.0 * (i * 2 + 1), 0.0, i);
    }

    for (size_t i = 0; i < simd_size<T>::value; ++i) {
        chain[1].coeffs.set_amplitude(0.5, i);
        chain[0]
            .state.set_frequency_note(96000.0, 4, 0 + i, 0.0, 440.0, 0.0, i);
    }

    ob_cinfo << title_fmt("plugin_chain") << std::endl << chain << std::endl;

    ob_cinfo << label_fmt("input_audio") << input_audio << std::endl;

    // process the oscillator in series on a per sample basis
    for (size_t i = 0; i < N; ++i) {
        output_audio[i] = chain(input_audio[i]);
    }

    ob_cinfo << label_fmt("oscillator output") << output_audio << std::endl;

    output_audio = chain(input_audio);

    ob_cinfo << label_fmt("oscillator output") << output_audio << std::endl;

    return true;
}

bool test_simd_oscillator() {
    ob_cinfo << title_fmt("oscillator float") << std::endl;
    test_simd_oscillator_one<float, 256>();
    return true;
}

template <typename T> T munger(T const &a, T const &b, T const &c, T const &d) {
    return a * b + b * c + c * d + d * sin(a) + sqrt(b);
}

/// set_item free function, set item 0 of non-vector v to a
template <typename T,
          typename std::enable_if<!is_simd<T>::value, bool>::type sfinae = true>
T &set_flattened_item(T &v, T const &a, size_t x = 0) {
    (void)x;
    v = a;
    return v;
}

template <typename T,
          typename std::enable_if<is_simd<T>::value, bool>::type sfinae = true>
T &set_flattened_item(T &v, typename simd_flattened_type<T>::type const &a,
                      size_t x = 0) {
    typename simd_flattened_type<T>::type *p =
        (typename simd_flattened_type<T>::type *)v.data();
    p[x] = a;
    return v;
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
    ob_cinfo << label_fmt(label) << v << std::endl;
}

template <typename T> bool test_one_simd(T &r) {
    ob_cinfo << label_fmt("Initializer") << std::endl;
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
    ob_cinfo << label_fmt("a+b+c+d") << r << std::endl;
    ob_cinfo << label_fmt("sqrt(r)") << sqrt(r) << std::endl;
    ob_cinfo << label_fmt("cos(r)") << cos(r) << std::endl;
    ob_cinfo << label_fmt("arg(r)") << arg(r) << std::endl;
    ob_cinfo << label_fmt("abs(r)") << abs(r) << std::endl;
    ob_cinfo << label_fmt("reciprocal(r)") << reciprocal(r) << std::endl;
    T ac = a;
    b = a + a;
    ob_cinfo << label_fmt("b") << (b) << std::endl;
    ob_cinfo << label_fmt("ac") << (ac) << std::endl;
    ob_cinfo << label_fmt("ac+=b") << (ac += b) << std::endl;
    ob_cinfo << label_fmt("ac-=b") << (ac -= b) << std::endl;
    ob_cinfo << label_fmt("ac*=b") << (ac *= b) << std::endl;
    ob_cinfo << label_fmt("ac/=b") << (ac /= b) << std::endl;

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

    vec4complex a4c;
    test_one_simd(a4c);

    audiochunk4channel ac4c;
    test_one_simd(ac4c);

    ac4c[1][0] = 99999;
    ob_cinfo << ac4c << std::endl;

    auto ref1 = make_simd_ref(a4d);
    ob_cinfo << label_fmt("ref1") << ref1 << std::endl;
    ob_cinfo << is_simd_ref<decltype(ref1)>::value << std::endl;
    ref1[0] = 9;
    ob_cinfo << label_fmt("ref") << ref1 << std::endl;

    vec4double const a4dc = a4d;
    auto ref2c = make_simd_ref(a4dc);
    ob_cinfo << label_fmt("ref2c") << ref2c << std::endl;
    ob_cinfo << is_simd_ref<decltype(ref2c)>::value << std::endl;

    auto ref3 = make_simd_ref(ref1);
    ob_cinfo << label_fmt("ref3") << ref3 << std::endl;

    auto ref4 = make_simd_ref(ref2c);
    ob_cinfo << label_fmt("ref4 of ref2") << ref4 << std::endl;

    vec4float a4f1 = a4f * 3.1415f;

    apply(a4f, [](float aa, float bb) { return aa + (bb * 2); }, a4f, a4f1);
    ob_cinfo << label_fmt("a4f") << a4f << std::endl;

    float m = munger(1.1f, 2.1f, 3.1f, 4.1f);
    ob_cinfo << label_fmt("m") << m << std::endl;

    vec4float m4 = munger(
        vec4float{1.1f, 1.2f, 1.3f, 1.4f}, vec4float{2.1f, 2.2f, 2.3f, 2.4f},
        vec4float{3.1f, 3.2f, 3.3f, 3.4f}, vec4float{4.1f, 4.2f, 4.3f, 4.4f});
    ob_cinfo << label_fmt("m4") << m4 << std::endl;

    vec4float m4a;
    vec4float m4a1 = {1.1f, 1.2f, 1.3f, 1.4f};
    vec4float m4a2 = {2.1f, 2.2f, 2.3f, 2.4f};
    vec4float m4a3 = {3.1f, 3.2f, 3.3f, 3.4f};
    vec4float m4a4 = {4.1f, 4.2f, 4.3f, 4.4f};

    apply(m4a, munger<float>, m4a1, m4a2, m4a3, m4a4);

    ob_cinfo << label_fmt("m4a") << m4a << std::endl;

    OB_RUN_TEST(test_simd_biquad, "SIMD");
    OB_RUN_TEST(test_simd_oscillator, "SIMD");

    return false;
}
}
}
