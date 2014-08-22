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

namespace Obbligato
{
namespace DSP
{

using namespace SIMD;

extern double oscillator_octave_multiplier_table[8];
extern double oscillator_note_frequencies_a440[12];
extern float oscillator_octave_multiplier_table_f[8];
extern float oscillator_note_frequencies_a440_f[12];

template <typename T>
struct Oscillator
{
    typedef T value_type;
    typedef typename simd_flattened_type<T>::type item_type;
    enum
    {
        vector_size = simd_size<T>::value,
        flattened_size = simd_flattened_size<T>::value
    };

    struct Coeffs
    {
        T amplitude;

        void set_amplitude( item_type const &v, size_t channel )
        {
            set_flattened_item( amplitude, v, channel );
        }
        friend std::ostream &operator<<( std::ostream &o, Coeffs const &v )
        {
            using namespace IOStream;
            o << "{ "
              << "amplitude=" << v.amplitude << " }";
            return o;
        }
    };

    struct State
    {

        T a, z1, z2;

        State()
        {
            zero( z1 );
            zero( z2 );
        }

        State( State const &other ) = default;

        State &operator=( State const &other ) = default;

        void set_frequency( float sample_rate_recip, float frequency, float phase_in_radians, size_t channel )
        {
            float w = static_cast<float>( OBBLIGATO_TWO_PI ) * frequency * sample_rate_recip;
            float temp2 = sinf( w + phase_in_radians );
            float tempa = 2.0f * cosf( w );
            item_type nz1;
            zero( nz1 );
            item_type nz2 = static_cast<item_type>( temp2 );
            item_type na = static_cast<item_type>( tempa );
            set_flattened_item( z1, nz1, channel );
            set_flattened_item( z2, nz2, channel );
            set_flattened_item( a, na, channel );
        }

        void set_frequency( double sample_rate_recip, double frequency, double phase_in_radians, size_t channel )
        {
            double w = ( OBBLIGATO_TWO_PI ) * frequency * sample_rate_recip;
            double temp2 = sin( w + phase_in_radians );
            double tempa = 2.0f * cos( w );
            item_type nz1;
            zero( nz1 );
            item_type nz2 = static_cast<item_type>( temp2 );
            item_type na = static_cast<item_type>( tempa );
            set_flattened_item( z1, nz1, channel );
            set_flattened_item( z2, nz2, channel );
            set_flattened_item( a, na, channel );
        }

        void set_frequency_note( double sample_rate_recip,
                                 int octave,
                                 int note,
                                 double tuning_in_cents = 0.0,
                                 double tuning_of_a = 440.0,
                                 double phase_in_radians = 0.0,
                                 size_t channel = 0 )
        {
            double tuning_multiplier = pow( 2.0, tuning_in_cents * ( 1.0 / 1200.0 ) );
            double octave_multiplier = oscillator_octave_multiplier_table[octave];
            double a_tuning_multipler = tuning_of_a * ( 1.0 / 440.0 );
            double freq = oscillator_note_frequencies_a440[note] * tuning_multiplier * octave_multiplier * a_tuning_multipler;
            set_frequency( sample_rate_recip, freq, phase_in_radians, channel );
        }

        void set_frequency_note( float sample_rate_recip,
                                 int octave,
                                 int note,
                                 float tuning_in_cents = 0.0f,
                                 float tuning_of_a = 440.0f,
                                 float phase_in_radians = 0.0f,
                                 size_t channel = 0 )
        {
            float tuning_multiplier = powf( 2.0f, tuning_in_cents * ( 1.0f / 1200.0f ) );
            float octave_multiplier = oscillator_octave_multiplier_table_f[octave];
            float a_tuning_multipler = tuning_of_a * ( 1.0f / 440.0f );
            float freq = oscillator_note_frequencies_a440_f[note] * tuning_multiplier * octave_multiplier * a_tuning_multipler;
            set_frequency( sample_rate_recip, freq, phase_in_radians, channel );
        }

        friend std::ostream &operator<<( std::ostream &o, State const &v )
        {
            using namespace IOStream;
            o << "{ "
              << "z1=" << v.z1 << " z2=" << v.z2 << " }";
            return o;
        }
    };

    Coeffs coeffs;
    State state;

    Oscillator() : coeffs(), state()
    {
    }

    Oscillator( Oscillator const &other ) : coeffs( other.coeffs ), state( other.state )
    {
    }

    Oscillator &operator=( Oscillator const &other )
    {
        coeffs = other.coeffs;
        state = other.state;
    }

    T operator()( T input_value )
    {
        T output_value;

        output_value = ( state.a * state.z1 - state.z2 );
        state.z2 = state.z1;
        state.z1 = output_value;

        return output_value * coeffs.amplitude + input_value;
    }

    friend std::ostream &operator<<( std::ostream &o, Oscillator const &v )
    {
        using namespace IOStream;
        o << label_fmt( "coeffs" ) << v.coeffs << std::endl;
        o << label_fmt( "state" ) << v.state << std::endl;
        return o;
    }
};
}
}
