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

template <typename T, size_t N> bool test_dsp_biquad_one() {
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

bool test_dsp_biquad() {
    ob_cinfo << title_fmt("biquad float") << std::endl;
    test_dsp_biquad_one<float, 8>();
    ob_cinfo << title_fmt("biquad float x 4") << std::endl;
    test_dsp_biquad_one<SIMD_Vector<float, 4>, 8>();
    ob_cinfo << title_fmt("biquad double") << std::endl;
    test_dsp_biquad_one<double, 8>();
    ob_cinfo << title_fmt("biquad double x 2") << std::endl;
    test_dsp_biquad_one<SIMD_Vector<double, 2>, 8>();
    return true;
}

template <typename T, size_t N> bool test_dsp_oscillator_one() {
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

bool test_dsp_oscillator() {
    ob_cinfo << title_fmt("oscillator float") << std::endl;
    test_dsp_oscillator_one<float, 256>();
    return true;
}

template <typename T, size_t N> bool test_dsp_gain_one() {
    SIMD_Vector<T, N> input_audio, output_audio;
    one(input_audio);
    zero(output_audio);

    typedef PluginChain<Gain<T>, T, 1> ChainType;
    ChainType chain;

    for (size_t i = 0; i < simd_size<T>::value; ++i) {
        typename Gain<T>::item_type z;
        one(z);
        for (size_t i = 0; i < N; ++i) {
            chain[0].coeffs.set_time_constant(96000.0, 0.050, i);
            chain[0].coeffs.set_amplitude(z);
        }
    }

    ob_cinfo << title_fmt("plugin_chain") << std::endl << chain << std::endl;
    ob_cinfo << label_fmt("input_audio") << input_audio << std::endl;

    // process the oscillator in series on a per sample basis
    for (size_t i = 0; i < N; ++i) {
        output_audio[i] = chain(input_audio[i]);
    }

    ob_cinfo << label_fmt("gain output") << output_audio << std::endl;

    for (size_t i = 0; i < 20; ++i) {
        output_audio = chain(input_audio);
        ob_cinfo << label_fmt("gain output") << output_audio << std::endl;
    }

    return true;
}

bool test_dsp_gain() {
    ob_cinfo << title_fmt("gain float") << std::endl;
    test_dsp_gain_one<float, 256>();
    return true;
}

bool test_dsp() {

    OB_RUN_TEST(test_dsp_biquad, "DSP");
    OB_RUN_TEST(test_dsp_oscillator, "DSP");
    OB_RUN_TEST(test_dsp_gain, "DSP");

    return false;
}
}
}
