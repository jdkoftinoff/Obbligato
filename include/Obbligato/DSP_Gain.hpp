#pragma once
/*
 Copyright (c) 2013, J.D. Koftinoff Software, Ltd.
 <jeffk@jdkoftinoff.com>
 http://www.jdkoftinoff.com/
 All rights reserved.

 Permission to use, copy, modify, and/or distribute this software for
 any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.

 THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 WARRANTIES
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

#if __cplusplus >= 201103L
namespace Obbligato
{
namespace DSP
{

using namespace SIMD;

template <typename T>
struct Gain
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
        T m_time_constant;
        T m_one_minus_time_constant;

        Coeffs( double sample_rate = 96000.0 )
        {
            item_type z;
            zero( z );
            for ( size_t i = 0; i < flattened_size; ++i )
            {
                setTimeConstant( sample_rate, 0.050, i );
                setAmplitude( z, i );
            }
        }

#if __cplusplus >= 201103L
        Coeffs( Coeffs const &other ) = default;

        Coeffs &operator=( Coeffs const &other ) = default;
#endif

        template <typename ComplexType>
        ComplexType processZDomain( size_t channel, ComplexType z1 )
        {
            return z1 * m_amplitude;
        }

        void setTimeConstant( double sample_rate,
                              double time_in_seconds,
                              size_t channel )
        {
            double samples = time_in_seconds * sample_rate;
            item_type v
                = static_cast<item_type>( reciprocal( samples ) );
            set_flattened_item( m_time_constant, v, channel );
            item_type a;
            one( a );
            set_flattened_item(
                m_one_minus_time_constant, a - v, channel );
        }

        void setAmplitude( item_type v, size_t channel )
        {
            set_flattened_item( m_amplitude, v, channel );
        }

        friend std::ostream &operator<<( std::ostream &o,
                                         Coeffs const &v )
        {
            using namespace IOStream;
            o << "{ "
              << "amplitude=" << v.m_amplitude
              << " time_constant=" << v.m_time_constant
              << " one_minus_time_constant="
              << v.m_one_minus_time_constant << " }";
            return o;
        }
    };

    struct State
    {
        T m_current_amplitude;

        State() { zero( m_current_amplitude ); }

#if __cplusplus >= 201103L
        State( State const &other ) = default;

        State &operator=( State const &other ) = default;
#endif
        friend std::ostream &operator<<( std::ostream &o,
                                         State const &v )
        {
            using namespace IOStream;
            o << "{ "
              << "current_amplitude=" << v.m_current_amplitude << " }";
            return o;
        }
    };

    Coeffs m_coeffs;
    State m_state;

    Gain() : m_coeffs(), m_state() {}

    Gain( Gain const &other )
        : m_coeffs( other.m_coeffs ), m_state( other.m_state )
    {
    }

    Gain &operator=( Gain const &other )
    {
        m_coeffs = other.m_coeffs;
        m_state = other.m_state;
    }

    T operator()( T input_value )
    {
        m_state.m_current_amplitude
            = ( m_coeffs.m_amplitude * m_coeffs.m_time_constant )
              + ( m_state.m_current_amplitude
                  * m_coeffs.m_one_minus_time_constant );

        return input_value * m_state.m_current_amplitude;
    }

    friend std::ostream &operator<<( std::ostream &o, Gain const &v )
    {
        using namespace IOStream;
        o << label_fmt( "coeffs" ) << v.m_coeffs << std::endl;
        o << label_fmt( "state" ) << v.m_state << std::endl;
        return o;
    }
};
}
}
#endif
