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

template <typename T> struct Gain {
    typedef T value_type;
    typedef typename simd_flattened_type<T>::type item_type;
    enum {
        vector_size = simd_size<T>::value,
        flattened_size = simd_flattened_size<T>::value
    };

    struct Coeffs {
        T amplitude;
        T time_constant;
        T one_minus_time_constant;

        Coeffs() {
            item_type z;
            zero(z);
            for (size_t i = 0; i < flattened_size; ++i) {
                set_time_constant(96000.0, 0.050, i);
                set_amplitude(z, i);
            }
        }

        Coeffs(Coeffs const &other) = default;

        Coeffs &operator=(Coeffs const &other) = default;

        void set_time_constant(double sample_rate, double time_in_seconds,
                               size_t channel) {
            double samples = time_in_seconds * sample_rate;
            item_type v = static_cast<item_type>(reciprocal(samples));
            set_flattened_item(time_constant, v, channel);
            item_type a;
            one(a);
            set_flattened_item(one_minus_time_constant, a - v, channel);
        }

        void set_amplitude(item_type v, size_t channel) {
            set_flattened_item(amplitude, v, channel);
        }

        friend std::ostream &operator<<(std::ostream &o, Coeffs const &v) {
            using namespace IOStream;
            o << "{ "
              << "amplitude=" << v.amplitude
              << "time_constant=" << v.time_constant
              << "one_minus_time_constant=" << v.one_minus_time_constant
              << " }";
            return o;
        }
    };

    struct State {

        T current_amplitude;

        State() { zero(current_amplitude); }

        State(State const &other) = default;

        State &operator=(State const &other) = default;

        friend std::ostream &operator<<(std::ostream &o, State const &v) {
            using namespace IOStream;
            o << "{ "
              << "current_amplitude=" << v.current_amplitude << " }";
            return o;
        }
    };

    Coeffs coeffs;
    State state;

    Gain() : coeffs(), state() {}

    Gain(Gain const &other) : coeffs(other.coeffs), state(other.state) {}

    Gain &operator=(Gain const &other) {
        coeffs = other.coeffs;
        state = other.state;
    }

    T operator()(T input_value) {
        state.current_amplitude =
            (coeffs.amplitude * coeffs.time_constant) +
            (state.current_amplitude * coeffs.one_minus_time_constant);

        return input_value * state.current_amplitude;
    }

    friend std::ostream &operator<<(std::ostream &o, Gain const &v) {
        using namespace IOStream;
        o << label_fmt("coeffs") << v.coeffs << std::endl;
        o << label_fmt("state") << v.state << std::endl;
        return o;
    }
};
}
}
