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
#include "Obbligato/IOStream.hpp"

namespace Obbligato {
namespace SIMD {

template <typename T, size_t N>
class OBBLIGATO_PLATFORM_VECTOR_ALIGN SIMD_Vector {
  public:
  
    /// The type of the vector
    typedef SIMD_Vector<T, N> simd_type;
    
    /// The type that the vector contains
    typedef T value_type;
    
    typedef T *pointer;
    typedef T const *const_pointer;
    typedef T &reference;
    typedef T const &const_reference;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    
    enum {
        /// The static size of the vector
        vector_size = N
    };

    /// The items of the vector
    value_type m_item[vector_size];

    /// Apply the function f on each item's value in the vector
    template <typename Func, typename ...Args>
    simd_type &apply( Func f, Args&&... args) {
        for( size_t i=0; i<vector_size; ++i ) {
            m_item[i] = f( m_item[i], std::forward<Args>(args)... );
        }
        return *this;
    }

    /// Default constructor does not initialize any values
    SIMD_Vector() {}

    /// The Initializer list constructor sets the values
    SIMD_Vector( std::initializer_list<value_type> list ) {
        size_t n=0;
        for( value_type const *v=list.begin(); v!=list.end() && n<vector_size; ++v ) {
            m_item[n++] = *v;
        }
    }

    /// Set all items to zero
    simd_type &zero() {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] = value_type(0);
        }
        return *this;
    }

    /// Set all items to one
    simd_type &one() {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] = value_type(1);
        }
        return *this;
    }

    /// Set all items to the value of v
    simd_type &splat(value_type const &v) {
        for( size_t i=0; i<vector_size; ++i ) {
            m_item[i] = v;
        }
        return *this;
    }

    /// Set a single item to V by index
    template <typename U> simd_type &set_item(U const &v, size_t index) {
        m_item[index] = v;
        return *this;
    }

    /// get a const ref to an item
    value_type const &get_item(size_t index) const { return m_item[index]; }

    /// get a non-const ref to an item
    value_type &get_item(size_t index) { return m_item[index]; }

    /// array index operator returns a const ref to the item
    value_type const &operator [](size_t index) const {
        return m_item[index];
    }

    /// array index operator returns a non-const ref to the item
    value_type &operator[](size_t index) {
        return m_item[index];
    }

    /// Copy constructor
    SIMD_Vector( simd_type const &other ) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] = other.m_item[i];
        }
    }

    /// Assignment operator
    simd_type const &operator=(simd_type const &other) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] = other.m_item[i];
        }
        return *this;
    }
    
    /// += operator
    simd_type const &operator+=(simd_type const &other) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] += other.m_item[i];
        }
        return *this;
    }

    /// -= operator
    simd_type const &operator-=(simd_type const &other) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] -= other.m_item[i];
        }
        return *this;
    }

    /// *= operator
    simd_type &operator*=(simd_type const &other) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] *= other.m_item[i];
        }
        return *this;
    }

    /// /= operator
    simd_type &operator/=(simd_type const &other) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] /= other.m_item[i];
        }
        return *this;
    }
};

/// Type traits to determine if a specified type is a simd type
template <typename T>
struct is_simd
    : std::false_type {
};

/// Partial template specialization for SIMD_Vector<T,N> to be shown as is_simd true
template <typename ItemT, size_t N>
struct is_simd<SIMD_Vector<ItemT, N>> :
    std::true_type {
    typedef SIMD_Vector<ItemT,N> simd_type;
    typedef typename SIMD_Vector<ItemT,N>::value_type value_type;
    enum {
        vector_size = N
    };
};

template <typename T>
struct is_not_simd
    : std::true_type {
    typedef T value_type;
    enum {
        vector_size = 1
    };
};

template <typename ItemT, size_t N>
struct is_not_simd<SIMD_Vector<ItemT, N>> :
    std::false_type {
};


/// Apply function Func on each associated item in all vectors
template <typename T, typename Func, typename ...Args>
typename std::enable_if< is_not_simd<T>::value,  T >::type &
apply( T &r, Func f, Args&&... args) {
    r = f( args... );
    return r;
}


/// Apply function Func on each associated item in all vectors
template <typename SimdT, typename Func, typename ...Args>
typename std::enable_if< is_simd<SimdT>::value, SimdT>::type &
apply( SimdT &r, Func f, Args&&... args) {
    for( size_t i=0; i<is_simd<SimdT>::vector_size; ++i ) {
        apply( r[i], f, args[i]... );
    }
    return r;
}


/// get_item free function, get const reference item 0 from non-vector v
template <typename T>
typename std::enable_if< is_not_simd<T>::value, T >::type const &
get_item(T const &v, size_t x=0) {
    (void)x;
    return v;
}

/// get_item free function, get const reference item x from vector v
template <typename T>
typename std::enable_if< is_simd<T>::value, typename is_simd<T>::value_type >::type const &
get_item(T const &v, size_t x) {
    return v[x];
}

/// get_item free function, get const reference item x,y from vector v
template <typename T>
typename std::enable_if< is_simd<T>::value, typename is_simd<T>::value_type >::type const &
get_item(T const &v, size_t x, size_t y) {
    return get_item( v[x], y );
}

/// get_item free function, get const reference item x,y,z from vector v
template <typename T>
typename std::enable_if< is_simd<T>::value, typename is_simd<T>::value_type >::type const &
get_item(T const &v, size_t x, size_t y, size_t z) {
    return get_item( v[x], y, z );
}

/// get_item free function, get reference item 0 from non-vector v
template <typename T>
typename std::enable_if< is_not_simd<T>::value, T >::type &
get_item(T &v, size_t x=0) {
    (void)x;
    return v;
}

/// get_item free function, get reference item x from vector v
template <typename T>
typename std::enable_if< is_simd<T>::value, typename is_simd<T>::value_type >::type &
get_item(T &v, size_t x) {
    return v[x];
}

/// get_item free function, get reference item x,y from vector v
template <typename T>
typename std::enable_if< is_simd<T>::value, typename is_simd<T>::value_type >::type &
get_item(T &v, size_t x, size_t y) {
    return get_item( v[x], y );
}

/// get_item free function, get reference item x,y,z from vector v
template <typename T>
typename std::enable_if< is_simd<T>::value, typename is_simd<T>::value_type >::type &
get_item(T &v, size_t x, size_t y, size_t z) {
    return get_item( v[x], y, z );
}

/// set_item free function, set item 0 of non-vector v to a
template <typename T>
typename std::enable_if< is_not_simd<T>::simd_type, T>::type &
set_item(T &v, T const &a, size_t x=0) {
    (void)x;
    v = a;
    return v;
}

/// set_item free function, set item x of vector v to a
template <typename T>
typename std::enable_if< is_simd<T>::simd_type, T>::type &
set_item(T &v,  typename std::enable_if< is_simd<T>::value, typename is_simd<T>::value_type >::type const &a, size_t x) {
    v.set_item(a, x);
    return v;
}

/// set_item free function, set item x,y of vector v to a
template <typename T>
typename std::enable_if< is_simd<T>::simd_type, T>::type &
set_item(T &v,  typename std::enable_if< is_simd<T>::value, typename is_simd<T>::value_type >::type const &a, size_t x, size_t y) {
    v.set_item(a[x], y);
    return v;
}

/// set_item free function, set item x,y of vector v to a
template <typename T>
typename std::enable_if< is_simd<T>::simd_type, T>::type &
set_item(T &v,  typename std::enable_if< is_simd<T>::value, typename is_simd<T>::value_type >::type const &a, size_t x, size_t y, size_t z) {
    v.set_item(a[x], y, z);
    return a;
}

/// Output the vector to the ostream
template <typename CharT, typename TraitsT, typename SimdT>
typename std::enable_if<
    is_simd<SimdT>::value,
    std::basic_ostream<CharT, TraitsT> >::type
 &operator<<(
    std::basic_ostream<CharT, TraitsT> &str, SimdT const &a ) {
    str << "{ ";
    for (size_t i = 0; i < is_simd<SimdT>::vector_size - 1; ++i) {
        str << get_item(a, i) << ", ";
    }
    str << get_item(a,is_simd<SimdT>::vector_size - 1) << " }";
    return str;
}

/// less than compare two vectors
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
operator < (SimdT const &a, SimdT const &b) {
    SimdT r;
    for (size_t i = 0; i < is_simd<SimdT>::vector_size; ++i) {
        r[i] = static_cast<typename is_simd<SimdT>::value_type>(a[i] < b[i]);
    }
    return r;
}

/// less than or equal for two vectors
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
operator <= (SimdT const &a, SimdT const &b) {
    SimdT r;
    for (size_t i = 0; i < is_simd<SimdT>::vector_size; ++i) {
        r[i] = static_cast<typename is_simd<SimdT>::value_type>(a[i] <= b[i]);
    }
    return r;
}

/// greater than or equal for two vectors
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
operator >= (SimdT const &a, SimdT const &b) {
    SimdT r;
    for (size_t i = 0; i < is_simd<SimdT>::vector_size; ++i) {
        r[i] = static_cast<typename is_simd<SimdT>::value_type>(a[i] >= b[i]);
    }
    return r;
}

/// not equal for two vectors
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
operator != (SimdT const &a, SimdT const &b) {
    SimdT r;
    for (size_t i = 0; i < is_simd<SimdT>::vector_size; ++i) {
        r[i] = static_cast<typename is_simd<SimdT>::value_type>(a[i] != b[i]);
    }
    return r;
}

/// equal for two vectors
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
operator == (SimdT const &a, SimdT const &b) {
    SimdT r;
    for (size_t i = 0; i < is_simd<SimdT>::vector_size; ++i) {
        r[i] = static_cast<typename is_simd<SimdT>::value_type>(a[i] == b[i]);
    }
    return r;
}

/// unary negation for one vector
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
operator - (SimdT const &a) {
    SimdT r;
    for (size_t i = 0; i < is_simd<SimdT>::vector_size; ++i) {
        r[i] = -a[i];
    }
    return r;
}

/// add two vectors
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
operator + (SimdT const &a, SimdT const &b) {
    SimdT r;
    for (size_t i = 0; i < is_simd<SimdT>::vector_size; ++i) {
        r[i] = (a[i] + b[i]);
    }
    return r;
}

/// subtract two vectors
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
operator - (SimdT const &a, SimdT const &b) {
    SimdT r;
    for (size_t i = 0; i < is_simd<SimdT>::vector_size; ++i) {
        r[i] = (a[i] - b[i]);
    }
    return r;
}

/// multiply two vectors
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
operator * (SimdT const &a, SimdT const &b) {
    SimdT r;
    for (size_t i = 0; i < is_simd<SimdT>::vector_size; ++i) {
        r[i] = (a[i] * b[i]);
    }
    return r;
}

/// divide vectors
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
operator / (SimdT const &a, SimdT const &b) {
    SimdT r;
    for (size_t i = 0; i < is_simd<SimdT>::vector_size; ++i) {
        r[i] = (a[i] / b[i]);
    }
    return r;
}

/// multiply add: a * b + c
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
multiply_add(SimdT const &a, SimdT const &b, SimdT &c ) {
    SimdT r;
    for (size_t i = 0; i < is_simd<SimdT>::vector_size; ++i) {
        r[i] = multiply_add(a[i],b[i],c[i]);
    }
    return r;
}

/// negative multiply subtract: - (a * b) - c
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
negative_multiply_subtract(SimdT const &a, SimdT const &b, SimdT &c ) {
    SimdT r;
    for (size_t i = 0; i < is_simd<SimdT>::vector_size; ++i) {
        r[i] = negative_multiply_subtrac(a[i],b[i],c[i]);
    }
    return r;
}

/// calculate sin() of each element
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
sin(SimdT const &a ) {
    SimdT r;
    return apply(r,sin,a);
}

/// calculate cos() of each element
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
cos(SimdT const &a ) {
    SimdT r;
    return apply(r,cos,a);
}

/// calculate abs() of each element
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
abs(SimdT const &a ) {
    SimdT r;
    return apply(r,abs,a);
}

/// calculate arg() of each element
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
arg(SimdT const &a ) {
    SimdT r;
    return apply(r,arg,a);
}

/// calculate sqrt() of each element
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
sqrt(SimdT const &a ) {
    SimdT r;
    return apply(r,sqrt,a);
}

/// calculate the reciprocal of each element
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
reciprocal(SimdT const &a ) {
    SimdT r;
    return apply(r,reciprocal,a);
}

/// calculate the reciprocal_estimate of each element
template <typename SimdT>
inline typename std::enable_if< is_simd<SimdT>::value, SimdT>::type
reciprocal_estimate(SimdT const &a ) {
    SimdT r;
    return apply(r,reciprocal_estimate,a);
}


}
}
