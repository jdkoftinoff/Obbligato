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

template <typename T>
struct Biquad
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
        T m_a0, m_a1, m_a2, m_b1, m_b2;

        template <typename ComplexType>
        ComplexType processZDomain( size_t channel, ComplexType z1 )
        {
            ComplexType one(1.0,0.0);
            ComplexType z2 = z1 * z1;

            T a0 = get_flattened_item(m_a0,channel);
            T a1 = get_flattened_item(m_a1,channel);
            T a2 = get_flattened_item(m_a2,channel);
            T b1 = get_flattened_item(m_b1,channel);
            T b2 = get_flattened_item(m_b2,channel);

            ComplexType numerator = ( a0 ) + ( a1 * z1 ) + ( a2 * z2 );
            ComplexType denominator = one + ( b1 * z1 ) + ( b2 * z2 );
            return numerator / denominator;
        }

        void set( size_t channel, double na0, double na1, double na2, double nb1, double nb2 )
        {
            set_flattened_item( m_a0, static_cast<item_type>( na0 ), channel );
            set_flattened_item( m_a1, static_cast<item_type>( na1 ), channel );
            set_flattened_item( m_a2, static_cast<item_type>( na2 ), channel );
            set_flattened_item( m_b1, static_cast<item_type>( nb1 ), channel );
            set_flattened_item( m_b2, static_cast<item_type>( nb2 ), channel );
        }

        void calculateLowpass( size_t channel, double sample_rate, double freq, double q )
        {
            double k = std::tan( OBBLIGATO_PI * freq / sample_rate );
            double norm = 1.0 / ( 1.0 + k / q + k * k );
            double na0 = k * k * norm;
            double na1 = 2.0 * na0;
            double na2 = na0;
            double nb1 = 2.0 * ( k * k - 1.0 ) * norm;
            double nb2 = ( 1.0 - k / q + k * k ) * norm;

            set( channel, na0, na1, na2, nb1, nb2 );
        }

        void calculateHighpass( size_t channel, double sample_rate, double freq, double q )
        {
            double k = std::tan( OBBLIGATO_PI * freq / sample_rate );
            double norm = 1.0 / ( 1.0 + k / q + k * k );
            double na0 = 1.0 * norm;
            double na1 = -2.0 * na0;
            double na2 = na0;
            double nb1 = 2.0 * ( k * k - 1.0 ) * norm;
            double nb2 = ( 1.0 - k / q + k * k ) * norm;
            set( channel, na0, na1, na2, nb1, nb2 );
        }

        void calculateBandpass( size_t channel, double sample_rate, double freq, double q )
        {
            double k = std::tan( OBBLIGATO_PI * freq / sample_rate );
            double norm = 1.0 / ( 1.0 + k / q + k * k );
            double na0 = k / q * norm;
            double na1 = 0.0;
            double na2 = -na0;
            double nb1 = 2.0 * ( k * k - 1.0 ) * norm;
            double nb2 = ( 1.0 - k / q + k * k ) * norm;
            set( channel, na0, na1, na2, nb1, nb2 );
        }

        void calculateNotch( size_t channel, double sample_rate, double freq, double q )
        {
            double k = std::tan( OBBLIGATO_PI * freq / sample_rate );
            double norm = 1.0 / ( 1.0 + k / 1.0 + k * k );
            double na0 = ( 1.0 + k * k ) * norm;
            double na1 = 2.0 * ( k * k - 1.0 ) * norm;
            double na2 = na0;
            double nb1 = na1;
            double nb2 = ( 1.0 - k / q + k * k ) * norm;
            set( channel, na0, na1, na2, nb1, nb2 );
        }

        void calculatePeak( size_t channel, double sample_rate, double freq, double q, double gain )
        {
            double k = std::tan( OBBLIGATO_PI * freq / sample_rate );
            double v = pow( 10.0, std::abs( gain ) / 20.0 );

            if ( gain >= 0 )
            {
                double norm = 1.0 / ( 1.0 + 1.0 / q * k + k * k );
                double na0 = ( 1.0 + v / q * k + k * k ) * norm;
                double na1 = 2.0 * ( k * k - 1.0 ) * norm;
                double na2 = ( 1.0 - v / q * k + k * k ) * norm;
                double nb1 = na1;
                double nb2 = ( 1.0 - 1.0 / q * k + k * k ) * norm;

                set( channel, na0, na1, na2, nb1, nb2 );
            }
            else
            {
                double norm = 1.0 / ( 1.0 + v / q * k + k * k );
                double na0 = ( 1.0 + 1.0 / q * k + k * k ) * norm;
                double na1 = 2.0 * ( k * k - 1.0 ) * norm;
                double na2 = ( 1.0 - 1.0 / q * k + k * k ) * norm;
                double nb1 = na1;
                double nb2 = ( 1.0 - v / q * k + k * k ) * norm;

                set( channel, na0, na1, na2, nb1, nb2 );
            }
        }

        void calculateLowshelf( size_t channel, double sample_rate, double freq, double gain )
        {
            double k = std::tan( OBBLIGATO_PI * freq / sample_rate );
            double v = pow( 10.0, std::abs( gain ) / 20.0 );
            double sqrt2 = OBBLIGATO_SQRT2;

            if ( gain >= 0 )
            {
                double norm = 1.0 / ( 1.0 + sqrt2 * k + k * k );
                double na0 = ( 1.0 + sqrt2 * v * k + v * k * k ) * norm;
                double na1 = 2.0 * ( v * k * k - 1.0 ) * norm;
                double na2 = ( 1.0 - sqrt2 * v * k + v * k * k ) * norm;
                double nb1 = 2.0 * ( k * k - 1.0 ) * norm;
                double nb2 = ( 1.0 - sqrt2 * k + k * k ) * norm;

                set( channel, na0, na1, na2, nb1, nb2 );
            }
            else
            {
                double norm = 1.0 / ( 1.0 + sqrt2 * v * k + v * k * k );
                double na0 = ( 1.0 + sqrt2 * k + k * k ) * norm;
                double na1 = 2.0 * ( k * k - 1.0 ) * norm;
                double na2 = ( 1.0 - sqrt2 * k + k * k ) * norm;
                double nb1 = 2.0 * ( v * k * k - 1.0 ) * norm;
                double nb2 = ( 1.0 - sqrt2 * v * k + v * k * k ) * norm;
                set( channel, na0, na1, na2, nb1, nb2 );
            }
        }

        void calculateHighshelf( size_t channel, double sample_rate, double freq, double gain )
        {
            double k = std::tan( OBBLIGATO_PI * freq / sample_rate );
            double v = pow( 10.0, std::abs( gain ) / 20.0 );
            double sqrt2 = OBBLIGATO_SQRT2;

            if ( gain >= 0 )
            {
                double norm = 1.0 / ( 1.0 + sqrt2 * k + k * k );
                double na0 = ( v + sqrt2 * v * k + k * k ) * norm;
                double na1 = 2.0 * ( k * k - v ) * norm;
                double na2 = ( v - sqrt2 * v * k + k * k ) * norm;
                double nb1 = 2.0 * ( k * k - 1 ) * norm;
                double nb2 = ( 1.0 - sqrt2 * k + k * k ) * norm;
                set( channel, na0, na1, na2, nb1, nb2 );
            }
            else
            {
                double norm = 1.0 / ( v + sqrt2 * v * k + k * k );
                double na0 = ( 1.0 + sqrt2 * k + k * k ) * norm;
                double na1 = 2.0 * ( k * k - 1 ) * norm;
                double na2 = ( 1.0 - sqrt2 * k + k * k ) * norm;
                double nb1 = 2.0 * ( k * k - v ) * norm;
                double nb2 = ( v - sqrt2 * v * k + k * k ) * norm;
                set( channel, na0, na1, na2, nb1, nb2 );
            }
        }

        friend std::ostream &operator<<( std::ostream &o, Coeffs const &v )
        {
            using namespace IOStream;
            o << "{ "
              << "a0=" << v.m_a0 << " a1=" << v.m_a1 << " a2=" << v.m_a2 << " b1=" << v.m_b1 << " b2=" << v.m_b2;
            return o;
        }
    };

    struct State
    {

        T m_z1, m_z2;

        State()
        {
            zero( m_z1 );
            zero( m_z2 );
        }

        State( State const &other ) = default;

        State &operator=( State const &other ) = default;

        friend std::ostream &operator<<( std::ostream &o, State const &v )
        {
            using namespace IOStream;
            o << "{ "
              << "z1=" << v.m_z1 << " z2=" << v.m_z2;
            return o;
        }
    };

    Coeffs m_coeffs;
    State m_state;

    Biquad() : m_coeffs(), m_state()
    {
    }

    Biquad( Biquad const &other ) : m_coeffs( other.m_coeffs ), m_state( other.m_state )
    {
    }

    Biquad &operator=( Biquad const &other )
    {
        m_coeffs = other.m_coeffs;
        m_state = other.m_state;
    }

    T operator()( T input_value )
    {
        T output_value;

        output_value = input_value * m_coeffs.m_a0 + m_state.m_z1;
        m_state.m_z1 = input_value * m_coeffs.m_a1 + m_state.m_z2 - m_coeffs.m_b1 * output_value;
        m_state.m_z2 = input_value * m_coeffs.m_a2 + m_coeffs.m_b2 * output_value;

        return output_value;
    }

    friend std::ostream &operator<<( std::ostream &o, Biquad const &v )
    {
        using namespace IOStream;
        o << label_fmt( "coeffs" ) << v.m_coeffs << std::endl;
        o << label_fmt( "state" ) << v.m_state << std::endl;
        return o;
    }
};
}
}
