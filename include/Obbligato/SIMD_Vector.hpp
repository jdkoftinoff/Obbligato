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
#include "Obbligato/IOStream.hpp"

namespace Obbligato
{
namespace SIMD
{

template <typename T, size_t N>
class OBBLIGATO_PLATFORM_VECTOR_ALIGN SIMD_Vector;
template <typename T, size_t N>
class OBBLIGATO_PLATFORM_VECTOR_ALIGN SIMD_VectorRef;
template <typename T, size_t N>
class OBBLIGATO_PLATFORM_VECTOR_ALIGN SIMD_VectorConstRef;

/** \addtogroup simd_splat splat */
/**@{*/

inline float &splat( float &v, float a )
{
    v = a;
    return v;
}

inline double &splat( double &v, float a )
{
    v = a;
    return v;
}

template <typename T>
inline std::complex<T> &splat( std::complex<T> &v,
                               std::complex<T> const &a )
{
    v = a;
    return v;
}

/**@}*/

/** \addtogroup simd_zero zero */
/**@{*/

inline float &zero( float &v )
{
    v = 0.0f;
    return v;
}

inline double &zero( double &v )
{
    v = 0.0;
    return v;
}

template <typename T>
inline std::complex<T> &zero( std::complex<T> &v )
{
    v = std::complex<T>( T( 0 ), T( 0 ) );
    return v;
}

/**@}*/

/** \addtogroup simd_one one */
/**@{*/

inline float &one( float &v )
{
    v = 1.0f;
    return v;
}

inline double &one( double &v )
{
    v = 1.0;
    return v;
}

template <typename T>
inline std::complex<T> &one( std::complex<T> &v )
{
    v = std::complex<T>( T( 1 ), T( 0 ) );
    return v;
}

/**@}*/

/** \addtogroup simd_reciprocal reciprocal */
/**@{*/

inline float reciprocal( float v ) { return 1.0f / v; }

inline double reciprocal( double v ) { return 1.0f / v; }

template <typename T>
inline std::complex<T> reciprocal( std::complex<T> const &v )
{
    std::complex<T> a;
    one( a );
    a = a / v;
    return a;
}

/**@}*/

/** \addtogroup simd_reciprocal_sqrt reciprocal_sqrt */
/**@{*/

inline float reciprocal_sqrt( float v )
{
    return 1.0f / std::sqrt( v );
}

inline double reciprocal_sqrt( double v )
{
    return 1.0 / std::sqrt( v );
}

template <typename T>
inline std::complex<T> reciprocal_sqrt( std::complex<T> const &v )
{
    std::complex<T> a;
    one( a );
    a = a / std::sqrt( v );
    return a;
}

/**@}*/

/** \addtogroup simd_compare_equal_to equal_to */
/**@{*/

inline float equal_to( float a, float b )
{
    return a == b ? 1.0f : 0.0f;
}

inline double equal_to( double a, double b )
{
    return a == b ? 1.0 : 0.0;
}

template <typename T>
inline std::complex<T> equal_to( std::complex<T> const &a,
                                 std::complex<T> const &b )
{
    return ( a == b ) ? std::complex<T>( 1, 0 )
                      : std::complex<T>( 0, 0 );
}

/**@}*/

/** \addtogroup simd_compare_not_equal_to not_equal_to */
/**@{*/

inline float not_equal_to( float a, float b )
{
    return a != b ? 1.0f : 0.0f;
}

inline double not_equal_to( double a, double b )
{
    return a != b ? 1.0 : 0.0;
}

template <typename T>
inline std::complex<T> not_equal_to( std::complex<T> const &a,
                                     std::complex<T> const &b )
{
    return ( a != b ) ? std::complex<T>( 1, 0 )
                      : std::complex<T>( 0, 0 );
}

/**@}*/

/** \addtogroup simd_compare_less less */
/**@{*/

inline float less( float a, float b ) { return a < b ? 1.0f : 0.0f; }

inline double less( double a, double b ) { return a < b ? 1.0 : 0.0; }

template <typename T>
inline std::complex<T> less( std::complex<T> const &a,
                             std::complex<T> const &b )
{
    return ( abs( a ) < abs( b ) ) ? std::complex<T>( 1, 0 )
                                   : std::complex<T>( 0, 0 );
}

/**@}*/

/** \addtogroup simd_compare_less_equal less_equal */
/**@{*/

inline float less_equal( float a, float b )
{
    return a < b ? 1.0f : 0.0f;
}

inline double less_equal( double a, double b )
{
    return a < b ? 1.0 : 0.0;
}

template <typename T>
inline std::complex<T> less_equal( std::complex<T> const &a,
                                   std::complex<T> const &b )
{
    return ( abs( a ) <= abs( b ) ) ? std::complex<T>( 1, 0 )
                                    : std::complex<T>( 0, 0 );
}

/**@}*/

/** \addtogroup simd_compare_greater greater */
/**@{*/

inline float greater( float a, float b ) { return a > b ? 1.0f : 0.0f; }

inline double greater( double a, double b )
{
    return a > b ? 1.0 : 0.0;
}

template <typename T>
inline std::complex<T> greater( std::complex<T> const &a,
                                std::complex<T> const &b )
{
    return ( abs( a ) > abs( b ) ) ? std::complex<T>( 1, 0 )
                                   : std::complex<T>( 0, 0 );
}

/**@}*/

/** \addtogroup simd_compare_greater_equal greater_equal */
/**@{*/

inline float greater_equal( float a, float b )
{
    return a >= b ? 1.0f : 0.0f;
}

inline double greater_equal( double a, double b )
{
    return a >= b ? 1.0 : 0.0;
}

template <typename T>
inline std::complex<T> greater_equal( std::complex<T> const &a,
                                      std::complex<T> const &b )
{
    return ( abs( a ) >= abs( b ) ) ? std::complex<T>( 1, 0 )
                                    : std::complex<T>( 0, 0 );
}

/**@}*/

/// \todo log log2 log10 exp exp2 exp10

using std::sqrt;
using std::arg;
using std::abs;
using std::sin;
using std::cos;

template <typename T, size_t N>
class OBBLIGATO_PLATFORM_VECTOR_ALIGN SIMD_Vector
{
  public:
    /// The type of the vector
    typedef SIMD_Vector<T, N> simd_type;

    /// The type that the vector contains
    typedef T value_type;

    typedef value_type *pointer;
    typedef value_type const *const_pointer;
    typedef value_type &reference;
    typedef value_type const &const_reference;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    enum
    {
        /// The static size of the vector
        vector_size = N
    };

    /// The items of the vector
    value_type m_item[vector_size];

    /// Default constructor does not initialize any values
    SIMD_Vector() {}
#if __cplusplus >= 201103L
    /// The Initializer list constructor sets the values
    SIMD_Vector( std::initializer_list<value_type> list )
    {
        size_t n = 0;
        for ( auto v = std::begin( list );
              v != std::end( list ) && n < vector_size;
              ++v )
        {
            m_item[n++] = *v;
        }
    }
#endif
    /// Get the vector size
    size_type size() const { return vector_size; }

    /// Get the vector maximum size
    size_type max_size() const { return vector_size; }

    /// Is it empty
    bool empty() const { return false; }

    /// Fill with a specific value
    void fill( value_type const &a )
    {
        for ( size_type i = 0; i < size(); ++i )
        {
            m_item[i] = a;
        }
    }

    /// Swap values in container with the other
    void swap( simd_type &other )
    {
        for ( size_type i = 0; i < size(); ++i )
        {
            std::swap( m_item[i], other.m_item[i] );
        }
    }

    /// Get underlying array
    pointer data() { return m_item; }

    /// Get underlying array const
    const_pointer data() const { return m_item; }

    /// array index operator returns a const ref to the item
    value_type const &operator[]( size_t index ) const
    {
        return m_item[index];
    }

    /// at() returns a non-const ref to the item, with range checking
    value_type &at( size_t index )
    {
        if ( index > size() )
        {
            throw std::out_of_range( "SIMD_Vector" );
        }
        return m_item[index];
    }

    /// at() returns a const ref to the item, with range checking
    value_type const &at( size_t index ) const
    {
        if ( index > size() )
        {
            throw std::out_of_range( "SIMD_Vector" );
        }
        return m_item[index];
    }

    /// array index operator returns a non-const ref to the item
    value_type &operator[]( size_t index ) { return m_item[index]; }

    /// Copy constructor
    SIMD_Vector( simd_type const &other )
    {
        for ( size_t i = 0; i < vector_size; ++i )
        {
            m_item[i] = other.m_item[i];
        }
    }

    /// Assignment operator
    simd_type const &operator=( simd_type const &other )
    {
        for ( size_t i = 0; i < vector_size; ++i )
        {
            m_item[i] = other.m_item[i];
        }
        return *this;
    }

    /// Get the first item
    reference front() { return m_item[0]; }

    /// Get the first item (const)
    const_reference front() const { return m_item[0]; }

    /// Get the last item
    reference back() { return m_item[vector_size - 1]; }

    /// Get the last item (const)
    const_reference back() const { return m_item[vector_size - 1]; }

    /// Get the iterator for the beginning
    iterator begin() { return &m_item[0]; }

    /// Get the const_iterator for the beginning
    const_iterator begin() const { return &m_item[0]; }

    /// Get the const_iterator for the beginning
    const_iterator cbegin() const { return &m_item[0]; }

    /// Get the iterator for the end (one item past the last item)
    iterator end() { return &m_item[vector_size]; }

    /// Get the const_iterator for the end (one item past the last item)
    const_iterator end() const { return &m_item[vector_size]; }

    /// Get the const_iterator for the end (one item past the last item)
    const_iterator cend() const { return &m_item[vector_size]; }

    /// Output the vector to the ostream
    template <typename CharT, typename TraitsT>
    friend std::basic_ostream<CharT, TraitsT> &
        operator<<( std::basic_ostream<CharT, TraitsT> &str,
                    simd_type const &a )
    {
        str << "{ ";
#if __cplusplus >= 201103L
        for ( auto i = std::begin( a ); i != std::end( a ); ++i )
        {
            str << *i << " ";
        }
#else
        for ( const simd_type::value_type *i = std::begin( a );
              i != std::end( a );
              ++i )
        {
            str << *i << " ";
        }
#endif
        str << " }";
        return str;
    }

    friend simd_type splat( simd_type &v, value_type a )
    {
        for ( int i = 0; i < vector_size; ++i )
        {
            v.m_item[i] = a;
        }
        return v;
    }

    friend simd_type zero( simd_type &v )
    {
        value_type t;
        zero( t );
        return splat( v, t );
    }

    friend simd_type one( simd_type &v )
    {
        value_type t;
        one( t );
        return splat( v, t );
    }

    friend simd_type sqrt( simd_type const &a )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = sqrt( a[i] );
        }
        return r;
    }

    friend simd_type arg( simd_type const &a )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = arg( a[i] );
        }
        return r;
    }

    friend simd_type abs( simd_type const &a )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = abs( a[i] );
        }
        return r;
    }

    friend simd_type sin( simd_type const &a )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = sin( a[i] );
        }
        return r;
    }

    friend simd_type cos( simd_type const &a )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = cos( a[i] );
        }
        return r;
    }

    friend simd_type reciprocal( simd_type const &a )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = reciprocal( a[i] );
        }
        return r;
    }

    friend simd_type operator-( simd_type const &a )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = -a[i];
        }
        return r;
    }

    friend simd_type operator+( simd_type const &a )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = +a[i];
        }
        return r;
    }

    friend simd_type operator+=( simd_type &a, value_type const &b )
    {
        for ( size_t i = 0; i < vector_size; ++i )
        {
            a[i] += b;
        }
        return a;
    }

    friend simd_type operator-=( simd_type &a, value_type const &b )
    {
        for ( size_t i = 0; i < vector_size; ++i )
        {
            a[i] -= b;
        }
        return a;
    }

    friend simd_type operator*=( simd_type &a, value_type const &b )
    {
        for ( size_t i = 0; i < vector_size; ++i )
        {
            a[i] *= b;
        }
        return a;
    }

    friend simd_type operator/=( simd_type &a, value_type const &b )
    {
        for ( size_t i = 0; i < vector_size; ++i )
        {
            a[i] /= b;
        }
        return a;
    }

    friend simd_type operator+( simd_type const &a,
                                value_type const &b )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = a[i] + b;
        }
        return r;
    }

    friend simd_type operator-( simd_type const &a,
                                value_type const &b )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = a[i] - b;
        }
        return r;
    }

    friend simd_type operator*( simd_type const &a,
                                value_type const &b )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = a[i] * b;
        }
        return r;
    }

    friend simd_type operator/( simd_type const &a,
                                value_type const &b )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = a[i] / b;
        }
        return r;
    }

    friend simd_type operator+=( simd_type &a, simd_type const &b )
    {
        for ( size_t i = 0; i < vector_size; ++i )
        {
            a[i] += b[i];
        }
        return a;
    }

    friend simd_type operator-=( simd_type &a, simd_type const &b )
    {
        for ( size_t i = 0; i < vector_size; ++i )
        {
            a[i] -= b[i];
        }
        return a;
    }

    friend simd_type operator*=( simd_type &a, simd_type const &b )
    {
        for ( size_t i = 0; i < vector_size; ++i )
        {
            a[i] *= b[i];
        }
        return a;
    }

    friend simd_type operator/=( simd_type &a, simd_type const &b )
    {
        for ( size_t i = 0; i < vector_size; ++i )
        {
            a[i] /= b[i];
        }
        return a;
    }

    friend simd_type operator+( simd_type const &a, simd_type const &b )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = a[i] + b[i];
        }
        return r;
    }

    friend simd_type operator-( simd_type const &a, simd_type const &b )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = a[i] - b[i];
        }
        return r;
    }

    friend simd_type operator*( simd_type const &a, simd_type const &b )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = a[i] * b[i];
        }
        return r;
    }

    friend simd_type operator/( simd_type const &a, simd_type const &b )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = a[i] / b[i];
        }
        return r;
    }

    friend simd_type equal_to( simd_type const &a, simd_type const &b )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = equal_to( a[i], b[i] );
        }
        return r;
    }

    friend simd_type not_equal_to( simd_type const &a,
                                   simd_type const &b )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = not_equal_to( a[i], b[i] );
        }
        return r;
    }

    friend simd_type less( simd_type const &a, simd_type const &b )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = less( a[i], b[i] );
        }
        return r;
    }

    friend simd_type less_equal( simd_type const &a,
                                 simd_type const &b )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = less_equal( a[i], b[i] );
        }
        return r;
    }

    friend simd_type greater( simd_type const &a, simd_type const &b )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = greater( a[i], b[i] );
        }
        return r;
    }

    friend simd_type greater_equal( simd_type const &a,
                                    simd_type const &b )
    {
        simd_type r;
        for ( size_t i = 0; i < vector_size; ++i )
        {
            r[i] = greater_equal( a[i], b[i] );
        }
        return r;
    }
};

template <typename T, size_t N>
class SIMD_VectorRef
{
  public:
    /// The type of the vector
    typedef SIMD_Vector<T, N> simd_type;
    typedef SIMD_VectorRef<T, N> simd_ref_type;

    /// The type that the vector contains
    typedef T value_type;

    typedef T *pointer;
    typedef T const *const_pointer;
    typedef T &reference;
    typedef T const &const_reference;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    enum
    {
        /// The static size of the vector
        vector_size = N
    };

    /// The items of the vector
    simd_type &m_ref;

    /// Default constructor does not initialize any values
    SIMD_VectorRef( simd_type &other ) : m_ref( other ) {}

    /// Get the vector size
    size_type size() const { return vector_size; }

    /// Get the vector maximum size
    size_type max_size() const { return vector_size; }

    /// Is it empty
    bool empty() const { return false; }

    /// Fill with a specific value
    void fill( value_type const &a ) { m_ref.fill( a ); }

    /// Swap values in container with the other
    template <typename OtherT>
    void swap( OtherT &other )
    {
        m_ref.swap( other );
    }

    /// Get underlying array
    pointer data() { return m_ref.data(); }

    /// Get underlying array const
    const_pointer data() const { return m_ref.data(); }

    /// array index operator returns a const ref to the item
    value_type const &operator[]( size_t index ) const
    {
        return m_ref[index];
    }

    /// at() returns a non-const ref to the item, with range checking
    value_type &at( size_t index ) { return m_ref.at( index ); }

    /// at() returns a const ref to the item, with range checking
    value_type const &at( size_t index ) const
    {
        return m_ref.at( index );
    }

    /// array index operator returns a non-const ref to the item
    value_type &operator[]( size_t index ) { return m_ref[index]; }

    /// Assignment operator
    template <typename OtherT>
    simd_ref_type const &operator=( OtherT const &other )
    {
        m_ref = other;
        return *this;
    }

    /// Get the first item
    reference front() { return m_ref.front(); }

    /// Get the first item (const)
    const_reference front() const { return m_ref.front(); }

    /// Get the last item
    reference back() { return m_ref.back(); }

    /// Get the last item (const)
    const_reference back() const { return m_ref.back(); }

    /// Get the iterator for the beginning
    iterator begin() { return m_ref.begin(); }

    /// Get the const_iterator for the beginning
    const_iterator begin() const { return m_ref.begin(); }

    /// Get the const_iterator for the beginning
    const_iterator cbegin() const { return m_ref.cbegin(); }

    /// Get the iterator for the end (one item past the last item)
    iterator end() { return m_ref.end(); }

    /// Get the const_iterator for the end (one item past the last item)
    const_iterator end() const { return m_ref.end(); }

    /// Get the const_iterator for the end (one item past the last item)
    const_iterator cend() const { return m_ref.cend(); }

    /// Output the vector to the ostream
    /// Output the vector to the ostream
    template <typename CharT, typename TraitsT>
    friend std::basic_ostream<CharT, TraitsT> &
        operator<<( std::basic_ostream<CharT, TraitsT> &str,
                    simd_ref_type const &a )
    {
        str << a.m_ref;
        return str;
    }

    friend simd_type splat( simd_ref_type &v, value_type a )
    {
        return splat( v.m_ref, a );
    }

    friend simd_type zero( simd_ref_type &v )
    {
        return zero( v.m_ref );
    }

    friend simd_type one( simd_ref_type &v ) { return one( v.m_ref ); }

    friend simd_type sin( simd_ref_type const &v )
    {
        return sin( v.m_ref );
    }

    friend simd_type cos( simd_ref_type const &v )
    {
        return cos( v.m_ref );
    }

    friend simd_type sqrt( simd_ref_type const &v )
    {
        return sqrt( v.m_ref );
    }

    friend simd_type abs( simd_ref_type const &v )
    {
        return abs( v.m_ref );
    }

    friend simd_type arg( simd_ref_type const &v )
    {
        return arg( v.m_ref );
    }

    friend simd_type reciprocal( simd_ref_type const &v )
    {
        return reciprocal( v.m_ref );
    }

    friend simd_type operator+=( simd_ref_type &a, value_type const &b )
    {
        a.m_ref += b;
        return a.m_ref;
    }

    friend simd_type operator+=( simd_ref_type &a,
                                 simd_ref_type const &b )
    {
        a.m_ref += b.m_ref;
        return a.m_ref;
    }

    friend simd_type operator+=( simd_type &a, simd_ref_type const &b )
    {
        a += b.m_ref;
        return a;
    }

    friend simd_type operator-=( simd_ref_type &a, value_type const &b )
    {
        a.m_ref -= b;
        return a.m_ref;
    }

    friend simd_type operator-=( simd_ref_type &a,
                                 simd_ref_type const &b )
    {
        a.m_ref -= b.m_ref;
        return a.m_ref;
    }

    friend simd_type operator-=( simd_type &a, simd_ref_type const &b )
    {
        a -= b.m_ref;
        return a;
    }

    friend simd_type operator*=( simd_ref_type &a, value_type const &b )
    {
        a.m_ref *= b;
        return a.m_ref;
    }

    friend simd_type operator*=( simd_ref_type &a,
                                 simd_ref_type const &b )
    {
        a.m_ref *= b.m_ref;
        return a.m_ref;
    }

    friend simd_type operator*=( simd_type &a, simd_ref_type const &b )
    {
        a *= b.m_ref;
        return a;
    }

    friend simd_type operator/=( simd_ref_type &a, value_type const &b )
    {
        a.m_ref /= b;
        return a.m_ref;
    }

    friend simd_type operator/=( simd_ref_type &a,
                                 simd_ref_type const &b )
    {
        a.m_ref /= b.m_ref;
        return a.m_ref;
    }

    friend simd_type operator/=( simd_type &a, simd_ref_type const &b )
    {
        a /= b.m_ref;
        return a;
    }

    friend simd_type operator+( simd_ref_type const &a )
    {
        simd_type r;
        r = +a.m_ref;
        return r;
    }

    friend simd_type operator-( simd_ref_type const &a )
    {
        simd_type r;
        r = -a.m_ref;
        return r;
    }

    friend simd_type operator+( simd_ref_type const &a,
                                value_type const &b )
    {
        simd_type r;
        r = a.m_ref + b;
        return r;
    }

    friend simd_type operator-( simd_ref_type const &a,
                                value_type const &b )
    {
        simd_type r;
        r = a.m_ref - b;
        return r;
    }

    friend simd_type operator*( simd_ref_type const &a,
                                value_type const &b )
    {
        simd_type r;
        r = a.m_ref * b;
        return r;
    }

    friend simd_type operator/( simd_ref_type const &a,
                                value_type const &b )
    {
        simd_type r;
        r = a.m_ref / b;
        return r;
    }

    friend simd_type operator+( simd_ref_type const &a,
                                simd_ref_type const &b )
    {
        simd_type r;
        r = a.m_ref + b.m_ref;
        return r;
    }

    friend simd_type operator-( simd_ref_type const &a,
                                simd_ref_type const &b )
    {
        simd_type r;
        r = a.m_ref - b.m_ref;
        return r;
    }

    friend simd_type operator*( simd_ref_type const &a,
                                simd_ref_type const &b )
    {
        simd_type r;
        r = a.m_ref * b.m_ref;
        return r;
    }

    friend simd_type operator/( simd_ref_type const &a,
                                simd_ref_type const &b )
    {
        simd_type r;
        r = a.m_ref / b.m_ref;
        return r;
    }

    friend simd_type operator+( simd_ref_type const &a,
                                simd_type const &b )
    {
        simd_type r;
        r = a.m_ref + b;
        return r;
    }

    friend simd_type operator-( simd_ref_type const &a,
                                simd_type const &b )
    {
        simd_type r;
        r = a.m_ref - b;
        return r;
    }

    friend simd_type operator*( simd_ref_type const &a,
                                simd_type const &b )
    {
        simd_type r;
        r = a.m_ref * b;
        return r;
    }

    friend simd_type operator/( simd_ref_type const &a,
                                simd_type const &b )
    {
        simd_type r;
        r = a.m_ref / b;
        return r;
    }

    friend simd_type operator+( simd_type const &a,
                                simd_ref_type const &b )
    {
        simd_type r;
        r = a + b.m_ref;
        return r;
    }

    friend simd_type operator-( simd_type const &a,
                                simd_ref_type const &b )
    {
        simd_type r;
        r = a - b.m_ref;
        return r;
    }

    friend simd_type operator*( simd_type const &a,
                                simd_ref_type const &b )
    {
        simd_type r;
        r = a * b.m_ref;
        return r;
    }

    friend simd_type operator/( simd_type const &a,
                                simd_ref_type const &b )
    {
        simd_type r;
        r = a / b.m_ref;
        return r;
    }

    friend simd_type equal_to( simd_ref_type const &a,
                               simd_ref_type const &b )
    {
        return equal_to( a.m_ref, b.m_ref );
    }

    friend simd_type not_equal_to( simd_ref_type const &a,
                                   simd_ref_type const &b )
    {
        return not_equal_to( a.m_ref, b.m_ref );
    }

    friend simd_type less( simd_ref_type const &a,
                           simd_ref_type const &b )
    {
        return less( a.m_ref, b.m_ref );
    }

    friend simd_type less_equal( simd_ref_type const &a,
                                 simd_ref_type const &b )
    {
        return less_equal( a.m_ref, b.m_ref );
    }

    friend simd_type greater( simd_ref_type const &a,
                              simd_ref_type const &b )
    {
        return greater( a.m_ref, b.m_ref );
    }

    friend simd_type greater_equal( simd_ref_type const &a,
                                    simd_ref_type const &b )
    {
        return greater_equal( a.m_ref, b.m_ref );
    }

    friend simd_type equal_to( simd_ref_type const &a,
                               simd_type const &b )
    {
        return equal_to( a.m_ref, b );
    }

    friend simd_type not_equal_to( simd_ref_type const &a,
                                   simd_type const &b )
    {
        return not_equal_to( a.m_ref, b );
    }

    friend simd_type less( simd_ref_type const &a, simd_type const &b )
    {
        return less( a.m_ref, b );
    }

    friend simd_type less_equal( simd_ref_type const &a,
                                 simd_type const &b )
    {
        return less_equal( a.m_ref, b );
    }

    friend simd_type greater( simd_ref_type const &a,
                              simd_type const &b )
    {
        return greater( a.m_ref, b );
    }

    friend simd_type greater_equal( simd_ref_type const &a,
                                    simd_type const &b )
    {
        return greater_equal( a.m_ref, b );
    }

    friend simd_type equal_to( simd_type const &a,
                               simd_ref_type const &b )
    {
        return equal_to( a, b.m_ref );
    }

    friend simd_type not_equal_to( simd_type const &a,
                                   simd_ref_type const &b )
    {
        return not_equal_to( a, b.m_ref );
    }

    friend simd_type less( simd_type const &a, simd_ref_type const &b )
    {
        return less( a, b.m_ref );
    }

    friend simd_type less_equal( simd_type const &a,
                                 simd_ref_type const &b )
    {
        return less_equal( a, b.m_ref );
    }

    friend simd_type greater( simd_type const &a,
                              simd_ref_type const &b )
    {
        return greater( a, b.m_ref );
    }

    friend simd_type greater_equal( simd_type const &a,
                                    simd_ref_type const &b )
    {
        return greater_equal( a, b.m_ref );
    }
};

template <typename T, size_t N>
class SIMD_VectorConstRef
{
  public:
    /// The type of the vector
    typedef SIMD_Vector<T, N> simd_type;
    typedef SIMD_VectorConstRef<T, N> simd_ref_type;

    /// The type that the vector contains
    typedef T value_type;

    typedef T *pointer;
    typedef T const *const_pointer;
    typedef T &reference;
    typedef T const &const_reference;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    enum
    {
        /// The static size of the vector
        vector_size = N
    };

    /// The items of the vector
    simd_type const &m_ref;

    /// Default constructor does not initialize any values
    SIMD_VectorConstRef( SIMD_Vector<T, N> const &other )
        : m_ref( other )
    {
    }

    /// Get the vector size
    size_type size() const { return vector_size; }

    /// Get the vector maximum size
    size_type max_size() const { return vector_size; }

    /// Is it empty
    bool empty() const { return false; }

    /// Get underlying array const
    const_pointer data() const { return m_ref.data(); }

    /// array index operator returns a const ref to the item
    value_type const &operator[]( size_t index ) const
    {
        return m_ref[index];
    }

    /// at() returns a const ref to the item, with range checking
    value_type const &at( size_t index ) const
    {
        return m_ref.at( index );
    }

    /// Get the first item (const)
    const_reference front() const { return m_ref.front(); }

    /// Get the last item (const)
    const_reference back() const { return m_ref.back(); }

    /// Get the const_iterator for the beginning
    const_iterator begin() const { return m_ref.begin(); }

    /// Get the const_iterator for the beginning
    const_iterator cbegin() const { return m_ref.cbegin(); }

    /// Get the const_iterator for the end (one item past the last item)
    const_iterator end() const { return m_ref.end(); }

    /// Get the const_iterator for the end (one item past the last item)
    const_iterator cend() const { return m_ref.cend(); }

    /// Output the vector to the ostream
    template <typename CharT, typename TraitsT>
    friend std::basic_ostream<CharT, TraitsT> &
        operator<<( std::basic_ostream<CharT, TraitsT> &str,
                    simd_ref_type const &a )
    {
        str << a.m_ref;
        return str;
    }

    friend simd_type sin( simd_ref_type const &v )
    {
        return sin( v.m_ref );
    }

    friend simd_type cos( simd_ref_type const &v )
    {
        return cos( v.m_ref );
    }

    friend simd_type sqrt( simd_ref_type const &v )
    {
        return sqrt( v.m_ref );
    }

    friend simd_type abs( simd_ref_type const &v )
    {
        return abs( v.m_ref );
    }

    friend simd_type arg( simd_ref_type const &v )
    {
        return arg( v.m_ref );
    }

    friend simd_type reciprocal( simd_ref_type const &v )
    {
        return reciprocal( v.m_ref );
    }

    friend simd_type operator+( simd_ref_type const &a )
    {
        simd_type r;
        r = +a.m_ref;
        return r;
    }

    friend simd_type operator-( simd_ref_type const &a )
    {
        simd_type r;
        r = -a.m_ref;
        return r;
    }

    friend simd_type operator+( simd_ref_type const &a,
                                value_type const &b )
    {
        simd_type r;
        r = a.m_ref + b;
        return r;
    }

    friend simd_type operator-( simd_ref_type const &a,
                                value_type const &b )
    {
        simd_type r;
        r = a.m_ref - b;
        return r;
    }

    friend simd_type operator*( simd_ref_type const &a,
                                value_type const &b )
    {
        simd_type r;
        r = a.m_ref * b;
        return r;
    }

    friend simd_type operator/( simd_ref_type const &a,
                                value_type const &b )
    {
        simd_type r;
        r = a.m_ref / b;
        return r;
    }

    friend simd_type operator+( simd_ref_type const &a,
                                simd_ref_type const &b )
    {
        simd_type r;
        r = a.m_ref + b.m_ref;
        return r;
    }

    friend simd_type operator-( simd_ref_type const &a,
                                simd_ref_type const &b )
    {
        simd_type r;
        r = a.m_ref - b.m_ref;
        return r;
    }

    friend simd_type operator*( simd_ref_type const &a,
                                simd_ref_type const &b )
    {
        simd_type r;
        r = a.m_ref * b.m_ref;
        return r;
    }

    friend simd_type operator/( simd_ref_type const &a,
                                simd_ref_type const &b )
    {
        simd_type r;
        r = a.m_ref / b.m_ref;
        return r;
    }

    friend simd_type operator+( simd_ref_type const &a,
                                simd_type const &b )
    {
        simd_type r;
        r = a.m_ref + b;
        return r;
    }

    friend simd_type operator-( simd_ref_type const &a,
                                simd_type const &b )
    {
        simd_type r;
        r = a.m_ref - b;
        return r;
    }

    friend simd_type operator*( simd_ref_type const &a,
                                simd_type const &b )
    {
        simd_type r;
        r = a.m_ref * b;
        return r;
    }

    friend simd_type operator/( simd_ref_type const &a,
                                simd_type const &b )
    {
        simd_type r;
        r = a.m_ref / b;
        return r;
    }

    friend simd_type operator+( simd_type const &a,
                                simd_ref_type const &b )
    {
        simd_type r;
        r = a + b.m_ref;
        return r;
    }

    friend simd_type operator-( simd_type const &a,
                                simd_ref_type const &b )
    {
        simd_type r;
        r = a - b.m_ref;
        return r;
    }

    friend simd_type operator*( simd_type const &a,
                                simd_ref_type const &b )
    {
        simd_type r;
        r = a * b.m_ref;
        return r;
    }

    friend simd_type operator/( simd_type const &a,
                                simd_ref_type const &b )
    {
        simd_type r;
        r = a / b.m_ref;
        return r;
    }

    friend simd_type equal_to( simd_ref_type const &a,
                               simd_ref_type const &b )
    {
        return equal_to( a.m_ref, b.m_ref );
    }

    friend simd_type not_equal_to( simd_ref_type const &a,
                                   simd_ref_type const &b )
    {
        return not_equal_to( a.m_ref, b.m_ref );
    }

    friend simd_type less( simd_ref_type const &a,
                           simd_ref_type const &b )
    {
        return less( a.m_ref, b.m_ref );
    }

    friend simd_type less_equal( simd_ref_type const &a,
                                 simd_ref_type const &b )
    {
        return less_equal( a.m_ref, b.m_ref );
    }

    friend simd_type greater( simd_ref_type const &a,
                              simd_ref_type const &b )
    {
        return greater( a.m_ref, b.m_ref );
    }

    friend simd_type greater_equal( simd_ref_type const &a,
                                    simd_ref_type const &b )
    {
        return greater_equal( a.m_ref, b.m_ref );
    }

    friend simd_type equal_to( simd_ref_type const &a,
                               simd_type const &b )
    {
        return equal_to( a.m_ref, b );
    }

    friend simd_type not_equal_to( simd_ref_type const &a,
                                   simd_type const &b )
    {
        return not_equal_to( a.m_ref, b );
    }

    friend simd_type less( simd_ref_type const &a, simd_type const &b )
    {
        return less( a.m_ref, b );
    }

    friend simd_type less_equal( simd_ref_type const &a,
                                 simd_type const &b )
    {
        return less_equal( a.m_ref, b );
    }

    friend simd_type greater( simd_ref_type const &a,
                              simd_type const &b )
    {
        return greater( a.m_ref, b );
    }

    friend simd_type greater_equal( simd_ref_type const &a,
                                    simd_type const &b )
    {
        return greater_equal( a.m_ref, b );
    }

    friend simd_type equal_to( simd_type const &a,
                               simd_ref_type const &b )
    {
        return equal_to( a, b.m_ref );
    }

    friend simd_type not_equal_to( simd_type const &a,
                                   simd_ref_type const &b )
    {
        return not_equal_to( a, b.m_ref );
    }

    friend simd_type less( simd_type const &a, simd_ref_type const &b )
    {
        return less( a, b.m_ref );
    }

    friend simd_type less_equal( simd_type const &a,
                                 simd_ref_type const &b )
    {
        return less_equal( a, b.m_ref );
    }

    friend simd_type greater( simd_type const &a,
                              simd_ref_type const &b )
    {
        return greater( a, b.m_ref );
    }

    friend simd_type greater_equal( simd_type const &a,
                                    simd_ref_type const &b )
    {
        return greater_equal( a, b.m_ref );
    }
};

/** \addtogroup simd_traits Traits
*/
/**@{*/

/// Type traits to determine if a specified type is a simd type
template <typename T>
struct is_simd : std::false_type
{
};

/// Partial template specialization for SIMD_Vector<T,N> to be shown as
/// is_simd
/// true
template <typename ItemT, size_t N>
struct is_simd<SIMD_Vector<ItemT, N> > : std::true_type
{
};

/// Partial template specialization for SIMD_VectorRef<T,N> to be shown
/// as
/// is_simd true
template <typename ItemT, size_t N>
struct is_simd<SIMD_VectorRef<ItemT, N> > : std::true_type
{
};

/// Partial template specialization for SIMD_VectorConstRef<T,N> to be
/// shown as
/// is_simd true
template <typename ItemT, size_t N>
struct is_simd<SIMD_VectorConstRef<ItemT, N> > : std::true_type
{
};

template <typename T>
struct is_simd_ref : std::false_type
{
};

/// Partial template specialization for SIMD_VectorConstRef<T,N> to be
/// shown as
/// is_simd true
template <typename ItemT, size_t N>
struct is_simd_ref<SIMD_VectorConstRef<ItemT, N> > : std::true_type
{
};

/// Partial template specialization for SIMD_VectorRef<T,N> to be shown
/// as
/// is_simd true
template <typename ItemT, size_t N>
struct is_simd_ref<SIMD_VectorRef<ItemT, N> > : std::true_type
{
};

template <typename T>
struct is_simd_const : std::false_type
{
};

template <typename ItemT, size_t N>
struct is_simd_const<SIMD_VectorConstRef<ItemT, N> > : std::true_type
{
};

template <typename ItemT, size_t N>
struct is_simd_const<const SIMD_Vector<ItemT, N> > : std::true_type
{
};

template <typename T>
struct simd_value_type
{
    typedef T type;
};

template <typename T, size_t N>
struct simd_value_type<SIMD_Vector<T, N> >
{
    typedef T type;
};

template <typename T, size_t N>
struct simd_value_type<SIMD_VectorRef<T, N> >
{
    typedef T type;
};

template <typename T, size_t N>
struct simd_value_type<SIMD_VectorConstRef<T, N> >
{
    typedef T type;
};

template <typename T>
struct simd_size : public std::integral_constant<size_t, 1>
{
};

template <typename T, size_t X>
struct simd_size<SIMD_Vector<T, X> >
    : public std::integral_constant<size_t, X>
{
};

template <typename T, size_t X>
struct simd_size<SIMD_VectorRef<T, X> >
    : public std::integral_constant<size_t, X>
{
};

template <typename T, size_t X>
struct simd_size<SIMD_VectorConstRef<T, X> >
    : public std::integral_constant<size_t, X>
{
};

template <typename T>
struct simd_flattened_size : public std::integral_constant<size_t, 1>
{
};

template <typename T, size_t X>
struct simd_flattened_size<SIMD_Vector<T, X> >
    : public std::integral_constant<size_t, X>
{
};

template <typename T, size_t X, size_t Y>
struct simd_flattened_size<SIMD_Vector<SIMD_Vector<T, Y>, X> >
    : public std::integral_constant<size_t, X * Y>
{
};

template <typename T, size_t X, size_t Y, size_t Z>
struct simd_flattened_size<SIMD_Vector<SIMD_Vector<SIMD_Vector<T, Z>,
                                                   Y>,
                                       X> >
    : public std::integral_constant<size_t, X * Y * Z>
{
};

template <typename T, size_t X, size_t Y, size_t Z, size_t W>
struct
    simd_flattened_size<SIMD_Vector<SIMD_Vector<SIMD_Vector<SIMD_Vector<T,
                                                                        W>,
                                                            Z>,
                                                Y>,
                                    X> >
    : public std::integral_constant<size_t, X * Y * Z * W>
{
};

template <typename T>
struct simd_flattened_type
{
    typedef T type;
};

template <typename T, size_t X>
struct simd_flattened_type<SIMD_Vector<T, X> >
{
    typedef T type;
};

template <typename T, size_t X, size_t Y>
struct simd_flattened_type<SIMD_Vector<SIMD_Vector<T, Y>, X> >
{
    typedef T type;
};

template <typename T, size_t X, size_t Y, size_t Z>
struct simd_flattened_type<SIMD_Vector<SIMD_Vector<SIMD_Vector<T, Z>,
                                                   Y>,
                                       X> >
{
    typedef T type;
};

template <typename T, size_t X, size_t Y, size_t Z, size_t W>
struct
    simd_flattened_type<SIMD_Vector<SIMD_Vector<SIMD_Vector<SIMD_Vector<T,
                                                                        W>,
                                                            Z>,
                                                Y>,
                                    X> >
{
    typedef T type;
};

/**@}*/

#if __cplusplus >= 201103L

template <typename SimdT,
          typename std::enable_if<!is_simd_ref<SimdT>::value,
                                  bool>::type sfinae = true>
SIMD_VectorRef<typename SimdT::value_type, SimdT::vector_size>
    make_simd_ref( SimdT &other )
{
    return SIMD_VectorRef<typename SimdT::value_type,
                          SimdT::vector_size>( other );
}

template <typename SimdT,
          typename std::enable_if<!is_simd_ref<SimdT>::value,
                                  bool>::type sfinae = true>
SIMD_VectorConstRef<typename SimdT::value_type, SimdT::vector_size>
    make_simd_ref( SimdT const &other )
{
    return SIMD_VectorConstRef<typename SimdT::value_type,
                               SimdT::vector_size>( other );
}

template <typename SimdT,
          typename std::enable_if<is_simd_ref<SimdT>::value, bool>::type
              sfinae1 = true,
          typename std::enable_if<!is_simd_const<SimdT>::value,
                                  bool>::type sfinae2 = true>
SIMD_VectorRef<typename SimdT::value_type, SimdT::vector_size>
    make_simd_ref( SimdT &other )
{
    return SIMD_VectorRef<typename SimdT::value_type,
                          SimdT::vector_size>( other.m_ref );
}

template <typename SimdT,
          typename std::enable_if<is_simd_ref<SimdT>::value, bool>::type
              sfinae = true>
SIMD_VectorConstRef<typename SimdT::value_type, SimdT::vector_size>
    make_simd_ref( SimdT const &other )
{
    return SIMD_VectorConstRef<typename SimdT::value_type,
                               SimdT::vector_size>( other.m_ref );
}

/** \addtogroup simd_apply Apply
*/
/**@{*/

/// Apply function Func on a non simd type
template <typename T, typename Func, typename... Args>
typename std::enable_if<!is_simd<T>::value, T>::type &
    apply( T &r, Func f, Args &&... args )
{
    r = f( args... );
    return r;
}

/// Apply function Func on each associated item in all vectors
template <typename SimdT, typename Func, typename... Args>
typename std::enable_if<is_simd<SimdT>::value, SimdT>::type &
    apply( SimdT &r, Func f, Args &&... args )
{
    for ( size_t i = 0; i < r.size(); ++i )
    {
        apply( r[i], f, args[i]... );
    }
    return r;
}

/**@}*/

/** \addtogroup simd_get get
*/

/**@{*/

/// get_item free function, get const reference item 0 from non-vector v
template <typename T,
          typename std::enable_if<!is_simd<T>::value, bool>::type sfinae
          = true>
T const &get_item( T const &v, size_t x = 0 )
{
    (void)x;
    return v;
}

/// get_item free function, get const reference item x from vector v
template <typename T,
          typename std::enable_if<is_simd<T>::value, bool>::type sfinae
          = true>
typename T::value_type const &get_item( T const &v, size_t x )
{
    return v[x];
}

/// get_item free function, get const reference item x,y from vector v
template <typename T,
          typename std::enable_if<is_simd<T>::value, bool>::type sfinae
          = true>
typename T::value_type const &get_item( T const &v, size_t x, size_t y )
{
    return get_item( v[x], y );
}

/// get_item free function, get const reference item x,y,z from vector v
template <typename T,
          typename std::enable_if<is_simd<T>::value, bool>::type sfinae
          = true>
typename T::value_type const &
    get_item( T const &v, size_t x, size_t y, size_t z )
{
    return get_item( v[x], y, z );
}

/// get_item free function, get reference item 0 from non-vector v
template <typename T,
          typename std::enable_if<!is_simd<T>::value, bool>::type sfinae
          = true>
T &get_item( T &v, size_t x = 0 )
{
    (void)x;
    return v;
}

/// get_item free function, get reference item x from vector v
template <typename T,
          typename std::enable_if<is_simd<T>::value, bool>::type sfinae
          = true>
typename T::value_type &get_item( T &v, size_t x )
{
    return v[x];
}

/// get_item free function, get reference item x,y from vector v
template <typename T,
          typename std::enable_if<is_simd<T>::value, bool>::type sfinae
          = true>
typename T::value_type &get_item( T &v, size_t x, size_t y )
{
    return get_item( v[x], y );
}

/// get_item free function, get reference item x,y,z from vector v
template <typename T,
          typename std::enable_if<is_simd<T>::value, bool>::type sfinae
          = true>
typename T::value_type &get_item( T &v, size_t x, size_t y, size_t z )
{
    return get_item( v[x], y, z );
}

/**@}*/

/** \addtogroup simd_set set
*/

/**@{*/

/// set_item free function, set item 0 of non-vector v to a
inline float &set_item( float &v, float const &a, size_t x = 0 )
{
    (void)x;
    v = a;
    return v;
}

inline double &set_item( double &v, double const &a, size_t x = 0 )
{
    (void)x;
    v = a;
    return v;
}

template <typename T>
inline std::complex<T> &set_item( std::complex<T> &v,
                                  std::complex<T> const &a,
                                  size_t x = 0 )
{
    (void)x;
    v = a;
    return v;
}

/// set_item free function, set item x of vector v to a
template <typename T,
          typename std::enable_if<is_simd<T>::value, bool>::type sfinae
          = true>
T &set_item( T &v, typename T::value_type const &a, size_t x )
{
    v[x] = a;
    return v;
}

/// set_item free function, set item x,y of vector v to a
template <typename T,
          typename std::enable_if<is_simd<T>::value, bool>::type sfinae
          = true>
T &set_item( T &v, typename T::value_type const &a, size_t x, size_t y )
{
    v[x][y] = a;
    return v;
}

/// set_item free function, set item x,y of vector v to a
template <typename T,
          typename std::enable_if<is_simd<T>::value, bool>::type sfinae
          = true>
T &set_item( T &v,
             typename T::value_type const &a,
             size_t x,
             size_t y,
             size_t z )
{
    v[x][y][z] = a;
    return a;
}
#endif

/**@}*/

/** \addtogroup simd_set_flattened_item set_flattened_item
*/

/**@{*/

inline float &set_flattened_item( float &v, float const &a, size_t i )
{
    (void)i;
    v = a;
    return v;
}

inline double &
    set_flattened_item( double &v, double const &a, size_t i )
{
    (void)i;
    v = a;
    return v;
}

template <typename T>
std::complex<T> &set_flattened_item( std::complex<T> &v,
                                     std::complex<T> const &a,
                                     size_t i )
{
    (void)i;
    v = a;
    return v;
}

template <typename SimdT>
SimdT &set_flattened_item(
    SimdT &v,
    typename simd_flattened_type<SimdT>::type const &a,
    size_t i )
{
    typename simd_flattened_type<SimdT>::type *f
        = (typename simd_flattened_type<SimdT>::type *)v.data();
    f[i] = a;
    return v;
}

/**@}*/

/** \addtogroup simd_get_flattened_item get_flattened_item
*/

/**@{*/

inline float const &get_flattened_item( float const &v, size_t i )
{
    (void)i;
    return v;
}

inline double const &get_flattened_item( double const &v, size_t i )
{
    (void)i;
    return v;
}

template <typename T>
std::complex<T> const &get_flattened_item( std::complex<T> const &v,
                                           size_t i )
{
    (void)i;
    return v;
}

template <typename SimdT>
typename simd_flattened_type<SimdT>::type const &
    get_flattened_item( SimdT const &v, size_t i )
{
    typename simd_flattened_type<SimdT>::type const *f
        = (typename simd_flattened_type<SimdT>::type const *)v.data();
    return f[i];
}

/**@}*/
}
}
