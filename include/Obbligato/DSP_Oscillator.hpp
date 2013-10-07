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
#include "Obbligato/Constants.hpp"

namespace Obbligato {
namespace DSP {

using namespace SIMD;

extern double oscillator_octave_multiplier_table[8];
extern double oscillator_note_frequencies_a440[12];

template <typename T> struct Oscillator {
    typedef T value_type;
    typedef typename simd_flattened_type<T>::type item_type;

    struct Coeffs {
        T amplitude;

        void set_amplitude(item_type const &v, size_t channel = 0) {
            set_item(amplitude, v, channel);
        }
        friend std::ostream &operator<<(std::ostream &o, Coeffs const &v) {
            using namespace IOStream;
            o << "{ "
              << "amplitude=" << v.amplitude << " }";
            return o;
        }
    };

    struct State {

        T a, z1, z2;

        State() {
            zero(z1);
            zero(z2);
        }

        State(State const &other) = default;

        State &operator=(State const &other) = default;

        void set_frequency(double sample_rate, double frequency,
                           double phase_in_radians, size_t channel = 0) {
            double w = (2.0 * (M_PI)) * frequency / sample_rate;
            double temp2 = sin(w + phase_in_radians);
            double tempa = 2.0f * cosf(w);
            item_type nz1;
            zero(nz1);
            item_type nz2 = static_cast<item_type>(temp2);
            item_type na = static_cast<item_type>(tempa);
            set_item(z1, nz1, channel);
            set_item(z2, nz2, channel);
            set_item(a, na, channel);
        }

        void set_frequency_note(double sample_rate, int octave, int note,
                                double tuning_in_cents = 0.0,
                                double tuning_of_a = 440.0,
                                double phase_in_radians = 0.0,
                                size_t channel = 0) {
            double tuning_multiplier =
                (tuning_in_cents - tuning_of_a) / tuning_of_a;
            double octave_multiplier =
                oscillator_octave_multiplier_table[octave];
            double freq = oscillator_note_frequencies_a440[note] *
                          tuning_multiplier * octave_multiplier;
            set_frequency(sample_rate, freq, phase_in_radians);
        }

        friend std::ostream &operator<<(std::ostream &o, State const &v) {
            using namespace IOStream;
            o << "{ "
              << "z1=" << v.z1 << " z2=" << v.z2 << " }";
            return o;
        }
    };

    Coeffs coeffs;
    State state;

    Oscillator() : coeffs(), state() {}

    Oscillator(Oscillator const &other)
        : coeffs(other.coeffs), state(other.state) {}

    Oscillator &operator=(Oscillator const &other) {
        coeffs = other.coeffs;
        state = other.state;
    }

    T operator()(T input_value) {
        T output_value;

        output_value = (state.a * state.z1 - state.z2);
        state.z2 = state.z1;
        state.z1 = output_value;

        return output_value * coeffs.amplitude + input_value;
    }

    friend std::ostream &operator<<(std::ostream &o, Oscillator const &v) {
        using namespace IOStream;
        o << label_fmt("coeffs") << v.coeffs << std::endl;
        o << label_fmt("state") << v.state << std::endl;
        return o;
    }
};
}
}
