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

template <typename T> struct Biquad {
    typedef T value_type;
    typedef typename simd_flattened_type<T>::type item_type;

    struct Coeffs {
        T a0, a1, a2, b1, b2;

        void set(size_t channel, double na0, double na1, double na2, double nb1,
                 double nb2) {
            set_item(a0, static_cast<item_type>(na0), channel);
            set_item(a1, static_cast<item_type>(na1), channel);
            set_item(a2, static_cast<item_type>(na2), channel);
            set_item(b1, static_cast<item_type>(nb1), channel);
            set_item(b2, static_cast<item_type>(nb2), channel);
        }

        void calculate_lowpass(size_t channel, double sample_rate, double freq,
                               double q) {
            double k = std::tan(M_PI * freq / sample_rate );
            double norm = 1.0 / (1.0 + k / q + k * k);
            double na0 = k * k * norm;
            double na1 = 2.0 * na0;
            double na2 = na0;
            double nb1 = 2.0 * (k * k - 1.0) * norm;
            double nb2 = (1.0 - k / q + k * k) * norm;

            set(channel, na0, na1, na2, nb1, nb2);
        }

        void calculate_highpass(size_t channel, double sample_rate, double freq,
                                double q) {
            double k = std::tan(M_PI * freq / sample_rate);
            double norm = 1.0 / (1.0 + k / q + k * k);
            double na0 = 1.0 * norm;
            double na1 = -2.0 * na0;
            double na2 = na0;
            double nb1 = 2.0 * (k * k - 1.0) * norm;
            double nb2 = (1.0 - k / q + k * k) * norm;
            set(channel, na0, na1, na2, nb1, nb2);
        }

        void calculate_bandpass(size_t channel, double sample_rate, double freq,
                                double q) {
            double k = std::tan(M_PI * freq / sample_rate);
            double norm = 1.0 / (1.0 + k / q + k * k);
            double na0 = k / q * norm;
            double na1 = 0.0;
            double na2 = -na0;
            double nb1 = 2.0 * (k * k - 1.0) * norm;
            double nb2 = (1.0 - k / q + k * k) * norm;
            set(channel, na0, na1, na2, nb1, nb2);
        }

        void calculate_notch(size_t channel, double sample_rate, double freq,
                             double q) {
            double k = std::tan(M_PI * freq / sample_rate);
            double norm = 1.0 / (1.0 + k / 1.0 + k * k);
            double na0 = (1.0 + k * k) * norm;
            double na1 = 2.0 * (k * k - 1.0) * norm;
            double na2 = na0;
            double nb1 = na1;
            double nb2 = (1.0 - k / q + k * k) * norm;
            set(channel, na0, na1, na2, nb1, nb2);
        }

        void calculate_peak(size_t channel, double sample_rate, double freq,
                            double q, double gain) {
            double k = std::tan(M_PI * freq / sample_rate);
            double v = pow(10.0, std::abs(gain) / 20.0);

            if (gain >= 0) {
                double norm = 1.0 / (1.0 + 1.0 / q * k + k * k);
                double na0 = (1.0 + v / q * k + k * k) * norm;
                double na1 = 2.0 * (k * k - 1.0) * norm;
                double na2 = (1.0 - v / q * k + k * k) * norm;
                double nb1 = na1;
                double nb2 = (1.0 - 1.0 / q * k + k * k) * norm;

                set(channel, na0, na1, na2, nb1, nb2);
            } else {
                double norm = 1.0 / (1.0 + v / q * k + k * k);
                double na0 = (1.0 + 1.0 / q * k + k * k) * norm;
                double na1 = 2.0 * (k * k - 1.0) * norm;
                double na2 = (1.0 - 1.0 / q * k + k * k) * norm;
                double nb1 = na1;
                double nb2 = (1.0 - v / q * k + k * k) * norm;

                set(channel, na0, na1, na2, nb1, nb2);
            }
        }

        void calculate_lowshelf(size_t channel, double sample_rate, double freq,
                                double q, double gain) {
            double k = std::tan(M_PI * freq / sample_rate);
            double v = pow(10.0, std::abs(gain) / 20.0);
            double sqrt2 = OBBLIGATO_SQRT2;

            if (gain >= 0) {
                double norm = 1.0 / (1.0 + sqrt2 * k + k * k);
                double na0 = (1.0 + sqrt2 * v * k + v * k * k) * norm;
                double na1 = 2.0 * (v * k * k - 1.0) * norm;
                double na2 = (1.0 - sqrt2 * v * k + v * k * k) * norm;
                double nb1 = 2.0 * (k * k - 1.0) * norm;
                double nb2 = (1.0 - sqrt2 * k + k * k) * norm;

                set(channel, na0, na1, na2, nb1, nb2);
            } else {
                double norm = 1.0 / (1.0 + sqrt2 * v * k + v * k * k);
                double na0 = (1.0 + sqrt2 * k + k * k) * norm;
                double na1 = 2.0 * (k * k - 1.0) * norm;
                double na2 = (1.0 - sqrt2 * k + k * k) * norm;
                double nb1 = 2.0 * (v * k * k - 1.0) * norm;
                double nb2 = (1.0 - sqrt2 * v * k + v * k * k) * norm;
                set(channel, na0, na1, na2, nb1, nb2);
            }
        }

        void calculate_highshelf(size_t channel, double sample_rate,
                                 double freq, double q, double gain) {
            double k = std::tan(M_PI * freq / sample_rate);
            double v = pow(10.0, std::abs(gain) / 20.0);
            double sqrt2 = OBBLIGATO_SQRT2;

            if (gain >= 0) {
                double norm = 1.0 / (1.0 + sqrt2 * k + k * k);
                double na0 = (v + sqrt2 * v * k + k * k) * norm;
                double na1 = 2.0 * (k * k - v) * norm;
                double na2 = (v - sqrt2 * v * k + k * k) * norm;
                double nb1 = 2.0 * (k * k - 1) * norm;
                double nb2 = (1.0 - sqrt2 * k + k * k) * norm;
                set(channel, na0, na1, na2, nb1, nb2);
            } else {
                double norm = 1.0 / (v + sqrt2 * v * k + k * k);
                double na0 = (1.0 + sqrt2 * k + k * k) * norm;
                double na1 = 2.0 * (k * k - 1) * norm;
                double na2 = (1.0 - sqrt2 * k + k * k) * norm;
                double nb1 = 2.0 * (k * k - v) * norm;
                double nb2 = (v - sqrt2 * v * k + k * k) * norm;
                set(channel, na0, na1, na2, nb1, nb2);
            }
        }

        friend std::ostream &operator<<(std::ostream &o, Coeffs const &v) {
            using namespace IOStream;
            o << "{ "
              << "a0=" << v.a0 << " a1=" << v.a1 << " a2=" << v.a2
              << " b1=" << v.b1 << " b2=" << v.b2;
            return o;
        }
    };

    struct State {

        T z1, z2;

        State() {
            zero(z1);
            zero(z2);
        }

        State(State const &other) = default;

        State &operator=(State const &other) = default;

        friend std::ostream &operator<<(std::ostream &o, State const &v) {
            using namespace IOStream;
            o << "{ "
              << "z1=" << v.z1 << " z2=" << v.z2;
            return o;
        }
    };

    Coeffs coeffs;
    State state;

    Biquad() : coeffs(), state() {}

    Biquad(Biquad const &other) : coeffs(other.coeffs), state(other.state) {}

    Biquad &operator=(Biquad const &other) {
        coeffs = other.coeffs;
        state = other.state;
    }

    T operator()(T input_value) {
        T output_value;

        output_value = input_value * coeffs.a0 + state.z1;
        state.z1 =
            input_value * coeffs.a1 + state.z2 - coeffs.b1 * output_value;
        state.z2 = input_value * coeffs.a2 + coeffs.b2 * output_value;

        return output_value;
    }

    friend std::ostream &operator<<(std::ostream &o, Biquad const &v) {
        using namespace IOStream;
        o << label_fmt("coeffs") << v.coeffs << std::endl;
        o << label_fmt("state") << v.state << std::endl;
        return o;
    }
};
}
}
