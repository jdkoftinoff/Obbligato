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
        T m_amplitude;

        void setAmplitude( item_type const &v, size_t channel ) { set_flattened_item( m_amplitude, v, channel ); }
        friend std::ostream &operator<<( std::ostream &o, Coeffs const &v )
        {
            using namespace IOStream;
            o << "{ "
              << "amplitude=" << v.m_amplitude << " }";
            return o;
        }
    };

    struct State
    {

        T m_a, m_z1, m_z2;

        State()
        {
            zero( m_z1 );
            zero( m_z2 );
        }

        State( State const &other ) = default;

        State &operator=( State const &other ) = default;

        void setFrequency( float sample_rate_recip, float frequency, float phase_in_radians, size_t channel )
        {
            float w = static_cast<float>( OBBLIGATO_TWO_PI ) * frequency * sample_rate_recip;
            float temp2 = sinf( w + phase_in_radians );
            float tempa = 2.0f * cosf( w );
            item_type nz1;
            zero( nz1 );
            item_type nz2 = static_cast<item_type>( temp2 );
            item_type na = static_cast<item_type>( tempa );
            set_flattened_item( m_z1, nz1, channel );
            set_flattened_item( m_z2, nz2, channel );
            set_flattened_item( m_a, na, channel );
        }

        void setFrequency( double sample_rate_recip, double frequency, double phase_in_radians, size_t channel )
        {
            double w = (OBBLIGATO_TWO_PI)*frequency * sample_rate_recip;
            double temp2 = sin( w + phase_in_radians );
            double tempa = 2.0f * cos( w );
            item_type nz1;
            zero( nz1 );
            item_type nz2 = static_cast<item_type>( temp2 );
            item_type na = static_cast<item_type>( tempa );
            set_flattened_item( m_z1, nz1, channel );
            set_flattened_item( m_z2, nz2, channel );
            set_flattened_item( m_a, na, channel );
        }

        void setFrequencyNote( double sample_rate_recip,
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
            setFrequency( sample_rate_recip, freq, phase_in_radians, channel );
        }

        void setFrequencyNote( float sample_rate_recip,
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
            setFrequency( sample_rate_recip, freq, phase_in_radians, channel );
        }

        friend std::ostream &operator<<( std::ostream &o, State const &v )
        {
            using namespace IOStream;
            o << "{ "
              << "z1=" << v.m_z1 << " z2=" << v.m_z2 << " }";
            return o;
        }
    };

    Coeffs m_coeffs;
    State m_state;

    Oscillator() : m_coeffs(), m_state() {}

    Oscillator( Oscillator const &other ) : m_coeffs( other.m_coeffs ), m_state( other.m_state ) {}

    Oscillator &operator=( Oscillator const &other )
    {
        m_coeffs = other.m_coeffs;
        m_state = other.m_state;
    }

    T operator()( T input_value )
    {
        T output_value;

        output_value = ( m_state.m_a * m_state.m_z1 - m_state.m_z2 );
        m_state.m_z2 = m_state.m_z1;
        m_state.m_z1 = output_value;

        return output_value * m_coeffs.m_amplitude + input_value;
    }

    friend std::ostream &operator<<( std::ostream &o, Oscillator const &v )
    {
        using namespace IOStream;
        o << label_fmt( "coeffs" ) << v.m_coeffs << std::endl;
        o << label_fmt( "state" ) << v.m_state << std::endl;
        return o;
    }
};
}
}
