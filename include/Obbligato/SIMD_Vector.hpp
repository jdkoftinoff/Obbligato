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
#include <complex>
#include <iterator>
#include <type_traits>

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
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    
    enum {
        /// The static size of the vector
        vector_size = N
    };

    /// The items of the vector
    value_type m_item[vector_size];

    /// Default constructor does not initialize any values
    SIMD_Vector() {}

    /// The Initializer list constructor sets the values
    SIMD_Vector( std::initializer_list<value_type> list ) {
        size_t n=0;
        for( auto v=std::begin(list); v!=std::end(list) && n<vector_size; ++v ) {
            m_item[n++] = *v;
        }
    }

    /// Get the vector size
    constexpr size_type size() const { return vector_size; }

    /// Get the vector maximum size
    constexpr size_type max_size() const { return vector_size; }
    
    /// Is it empty
    constexpr bool empty() const { return false; }
    
    /// Fill with a specific value
    void fill( value_type const &a ) {
        for( size_type i=0; i<size(); ++i ) {
            m_item[i] = a;
        }
    }
    
    /// Swap values in container with the other
    void swap( simd_type & other ) noexcept {
        using namespace std;
        for( size_type i=0; i<size(); ++i ) {
            swap( m_item[i], other.m_item[i] );
        }
    }
    
    /// Get underlying array
    pointer data() { return m_item; }
    
    /// Get underlying array const
    const_pointer data() const { return m_item; }
    
    /// array index operator returns a const ref to the item
    value_type const &operator [](size_t index) const {
        return m_item[index];
    }

    /// at() returns a non-const ref to the item, with range checking
    value_type &at(size_t index) {
        if( index>size()) {
            throw std::out_of_range("SIMD_Vector");
        }
        return m_item[index];
    }

    /// at() returns a const ref to the item, with range checking
    value_type const &at(size_t index) const {
        if( index>size()) {
            throw std::out_of_range("SIMD_Vector");
        }
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

    /// Get the first item
    reference front() {
        return m_item[0];
    }
    
    /// Get the first item (const)
    const_reference front() const {
        return m_item[0];
    }

    /// Get the last item
    reference back() {
        return m_item[vector_size-1];
    }
    
    /// Get the last item (const)
    const_reference back() const {
        return m_item[vector_size-1];
    }
    
    /// Get the iterator for the beginning
    iterator begin() {
        return &m_item[0];
    }
    
    /// Get the const_iterator for the beginning
    const_iterator begin() const {
        return &m_item[0];
    }
    
    /// Get the const_iterator for the beginning
    const_iterator cbegin() const {
        return &m_item[0];
    }

    /// Get the iterator for the end (one item past the last item)
    iterator end() {
        return &m_item[vector_size];
    }

    /// Get the const_iterator for the end (one item past the last item)
    const_iterator end() const {
        return &m_item[vector_size];
    }

    /// Get the const_iterator for the end (one item past the last item)
    const_iterator cend() const {
        return &m_item[vector_size];
    }
    
};


template <typename T, size_t N>
class SIMD_VectorRef {
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
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    
    enum {
        /// The static size of the vector
        vector_size = N
    };

    /// The items of the vector
    simd_type &m_ref;

    /// Default constructor does not initialize any values
    SIMD_VectorRef( simd_type &other )
        : m_ref(other) {}

    /// Get the vector size
    constexpr size_type size() const { return vector_size; }

    /// Get the vector maximum size
    constexpr size_type max_size() const { return vector_size; }
    
    /// Is it empty
    constexpr bool empty() const { return false; }
    
    /// Fill with a specific value
    void fill( value_type const &a ) {
        m_ref.fill(a);
    }
    
    /// Swap values in container with the other
    template <typename OtherT>
    void swap( OtherT & other ) noexcept {
        m_ref.swap( other );
    }
    
    /// Get underlying array
    pointer data() { return m_ref.data(); }
    
    /// Get underlying array const
    const_pointer data() const { return m_ref.data(); }
    
    /// array index operator returns a const ref to the item
    value_type const &operator [](size_t index) const {
        return m_ref[index];
    }

    /// at() returns a non-const ref to the item, with range checking
    value_type &at(size_t index) {
        return m_ref.at(index);
    }

    /// at() returns a const ref to the item, with range checking
    value_type const &at(size_t index) const {
        return m_ref.at(index);
    }

    /// array index operator returns a non-const ref to the item
    value_type &operator[](size_t index) {
        return m_ref[index];
    }

    /// Assignment operator
    template <typename OtherT>
    simd_type const &operator=(OtherT const &other) {
        m_ref = other;
        return *this;
    }

    /// Get the first item
    reference front() {
        return m_ref.front();
    }
    
    /// Get the first item (const)
    const_reference front() const {
        return m_ref.front();
    }

    /// Get the last item
    reference back() {
        return m_ref.back();
    }
    
    /// Get the last item (const)
    const_reference back() const {
        return m_ref.back();
    }
    
    /// Get the iterator for the beginning
    iterator begin() {
        return m_ref.begin();
    }
    
    /// Get the const_iterator for the beginning
    const_iterator begin() const {
        return m_ref.begin();
    }
    
    /// Get the const_iterator for the beginning
    const_iterator cbegin() const {
        return m_ref.cbegin();
    }

    /// Get the iterator for the end (one item past the last item)
    iterator end() {
        return m_ref.end();
    }

    /// Get the const_iterator for the end (one item past the last item)
    const_iterator end() const {
        return m_ref.end();
    }

    /// Get the const_iterator for the end (one item past the last item)
    const_iterator cend() const {
        return m_ref.cend();
    }
    
};


/** \addtogroup simd_traits Traits
*/
/**@{*/

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

/// Partial template specialization for SIMD_VectorRef<T,N> to be shown as is_simd true
template <typename ItemT, size_t N>
struct is_simd<SIMD_VectorRef<ItemT, N>> :
    std::true_type {
    typedef SIMD_VectorRef<ItemT,N> simd_type;
    typedef typename SIMD_VectorRef<ItemT,N>::value_type value_type;
    enum {
        vector_size = N
    };
};


/// Partial template specialization for std::array<T,N> to be shown as is_simd true
template <typename T, size_t N>
struct is_simd<std::array<T,N>> :
    std::true_type {
    typedef std::array<T,N> simd_type;
    typedef typename std::array<T,N>::value_type value_type;
    enum {
        vector_size = N
    };
};

/// Type traits to determine if a specified type is NOT a simd type
template <typename T>
struct is_not_simd
    : std::conditional< is_simd<T>::value, std::false_type, std::true_type>::type {
};

/**@}*/

template <typename SimdT>
SIMD_VectorRef< typename is_simd<SimdT>::value_type, is_simd<SimdT>::vector_size >
make_simd_ref( SimdT &other ) {
    return SIMD_VectorRef< typename is_simd<SimdT>::value_type, is_simd<SimdT>::vector_size >( other );
}

/** \addtogroup simd_apply Apply
*/
/**@{*/

/// Apply function Func on a non simd type
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
    for (size_t i = 0; i < r.size(); ++i) {
        apply( r[i], f, args[i]... );
    }
    return r;
}

/**@}*/

/** \addtogroup simd_get get
*/

/**@{*/

/// get_item free function, get const reference item 0 from non-vector v
template <typename T, typename std::enable_if< is_not_simd<T>::value, bool>::type sfinae=true >
T const &get_item(T const &v, size_t x=0) {
    (void)x;
    return v;
}

/// get_item free function, get const reference item x from vector v
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
typename is_simd<T>::value_type const &
get_item(T const &v, size_t x) {
    return v[x];
}

/// get_item free function, get const reference item x,y from vector v
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
typename is_simd<T>::value_type const &
get_item(T const &v, size_t x, size_t y) {
    return get_item( v[x], y );
}

/// get_item free function, get const reference item x,y,z from vector v
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
typename is_simd<T>::value_type const &
get_item(T const &v, size_t x, size_t y, size_t z) {
    return get_item( v[x], y, z );
}

/// get_item free function, get reference item 0 from non-vector v
template <typename T, typename std::enable_if< is_not_simd<T>::value, bool>::type sfinae=true >
T &get_item(T &v, size_t x=0) {
    (void)x;
    return v;
}

/// get_item free function, get reference item x from vector v
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
typename is_simd<T>::value_type &
get_item(T &v, size_t x) {
    return v[x];
}

/// get_item free function, get reference item x,y from vector v
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
typename is_simd<T>::value_type &
get_item(T &v, size_t x, size_t y) {
    return get_item( v[x], y );
}

/// get_item free function, get reference item x,y,z from vector v
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
typename is_simd<T>::value_type &
get_item(T &v, size_t x, size_t y, size_t z) {
    return get_item( v[x], y, z );
}

/**@}*/

/** \addtogroup simd_set set
*/

/**@{*/

/// set_item free function, set item 0 of non-vector v to a
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
typename is_simd<T>::value_type &
set_item(T &v, T const &a, size_t x=0) {
    (void)x;
    v = a;
    return v;
}

/// set_item free function, set item x of vector v to a
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T & set_item(T &v,  typename is_simd<T>::value_type const &a, size_t x) {
    v[x] = a;
    return v;
}

/// set_item free function, set item x,y of vector v to a
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T & set_item(T &v,  typename is_simd<T>::value_type const &a, size_t x, size_t y) {
    v[x][y] = a;
    return v;
}

/// set_item free function, set item x,y of vector v to a
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T & set_item(T &v,  typename is_simd<T>::value_type const &a, size_t x, size_t y, size_t z) {
    v[x][y][z] = a;
    return a;
}

/**@}*/

/** \addtogroup simd_splat splat
*/
/**@{*/

/// splat free function for non-vectors
template <typename T, typename std::enable_if< is_not_simd<T>::value, bool>::type sfinae=true >
T & simd_splat(T &v,  T const &a) {
    v=a;
    return v;
}

/// splat free function, set all items of v to a
template <typename T, typename U, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T & simd_splat(T &v,  U const &a) {
    for( auto i = std::begin(v); i!=std::end(v); ++i ) {
        simd_splat(*i, a );
    }
    return v;
}

/**@}*/

/** \addtogroup simd_zero zero */
/**@{*/

/// zero free function, set non-vector to zero
template <typename T, typename std::enable_if< is_not_simd<T>::value, bool>::type sfinae=true >
T & simd_zero(T &v) {
    v = T(0);
    return v;
}

/// zero free function, set all items of v to 0
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T & simd_zero(T &v) {
    for( auto i = std::begin(v); i!=std::end(v); ++i ) {
        simd_zero( *i );
    }
    return v;
}

/**@}*/

/** \addtogroup simd_one one */
/**@{*/

/// one free function, set non-vector to 1
template <typename T, typename std::enable_if< is_not_simd<T>::value, bool>::type sfinae=true >
T & simd_one(T &v) {
    v = T(1);
    return v;
}

/// one free function, set all items of v to 1
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T & simd_one(T &v) {
    for( auto i = std::begin(v); i!=std::end(v); ++i ) {
        simd_one( *i );
    }
    return v;
}

/**@}*/

/** \addtogroup simd_iostream iostream */
/**@{*/

/// Output the vector to the ostream
template <typename CharT, typename TraitsT, typename SimdT, typename std::enable_if< is_simd<SimdT>::value, bool>::type sfinae=true >
std::basic_ostream<CharT, TraitsT> &operator<<(
    std::basic_ostream<CharT, TraitsT> &str,
    SimdT const &a ) {
    str << "{ ";
    for ( auto i = std::begin(a); i!=std::end(a); ++i ) {
        str << *i << " ";
    }
    str << " }";
    return str;
}

/**@}*/

/** \addtogroup simd_comparisons comparisons */
/**@{*/

/// less than compare two vectors
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T operator < (T const &a, T const &b) {
    T r;
    for (size_t i = 0; i < r.size(); ++i) {
        r[i] = static_cast<typename is_simd<T>::value_type>(a[i] < b[i]);
    }
    return r;
}

/// less than or equal for two vectors
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T operator <= (T const &a, T const &b) {
    T r;
    for (size_t i = 0; i < r.size(); ++i) {
        r[i] = static_cast<typename is_simd<T>::value_type>(a[i] <= b[i]);
    }
    return r;
}

/// greater than for two vectors
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T operator > (T const &a, T const &b) {
    T r;
    for (size_t i = 0; i < r.size(); ++i) {
        r[i] = static_cast<typename is_simd<T>::value_type>(a[i] > b[i]);
    }
    return r;
}

/// greater than or equal for two vectors
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T operator >= (T const &a, T const &b) {
    T r;
    for (size_t i = 0; i < r.size(); ++i) {
        r[i] = static_cast<typename is_simd<T>::value_type>(a[i] >= b[i]);
    }
    return r;
}

/// not equal for two vectors
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T operator != (T const &a, T const &b) {
    T r;
    for (size_t i = 0; i < r.size(); ++i) {
        r[i] = static_cast<typename is_simd<T>::value_type>(a[i] != b[i]);
    }
    return r;
}

/// equal for two vectors
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T operator == (T const &a, T const &b) {
    T r;
    for (size_t i = 0; i < r.size(); ++i) {
        r[i] = static_cast<typename is_simd<T>::value_type>(a[i] == b[i]);
    }
    return r;
}
/**@}*/

/** \addtogroup simd_math math */
/**@{*/

/// unary negation for one vector
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T operator - (T const &a) {
    T r;
    for (size_t i = 0; i < a.size(); ++i) {
        r[i] = -a[i];
    }
    return r;
}

/// +=
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T const &operator += (T &a, T const &b) {
    for (size_t i = 0; i < a.size(); ++i) {
        a[i]+= b[i];
    }
    return a;
}

/// -=
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T const &operator -= (T &a, T const &b) {
    for (size_t i = 0; i < a.size(); ++i) {
        a[i]-= b[i];
    }
    return a;
}

/// *=
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T const &operator *= (T &a, T const &b) {
    for (size_t i = 0; i < a.size(); ++i) {
        a[i] *= b[i];
    }
    return a;
}

/// /=
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T const &operator /= (T &a, T const &b) {
    for (size_t i = 0; i < a.size(); ++i) {
        a[i] /= b[i];
    }
    return a;
}


/// add two vectors
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T operator + (T const &a, T const &b) {
    T r;
    for (size_t i = 0; i < r.size(); ++i) {
        r[i] = a[i] + b[i];
    }
    return r;
}

/// subtract two vectors
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T operator - (T const &a, T const &b) {
    T r;
    for (size_t i = 0; i < r.size(); ++i) {
        r[i] = a[i] - b[i];
    }
    return r;
}

/// multiply two vectors
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T operator * (T const &a, T const &b) {
    T r;
    for (size_t i = 0; i < r.size(); ++i) {
        r[i] = a[i] * b[i];
    }
    return r;
}

/// divide vectors
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T operator / (T const &a, T const &b) {
    T r;
    for (size_t i = 0; i < r.size(); ++i) {
        r[i] = a[i] / b[i];
    }
    return r;
}

/// multiply add: a * b + c
template <typename T, typename std::enable_if< is_not_simd<T>::value, bool>::type sfinae=true >
T multiply_add( T const &a, T const &b, T const &c ) {
    return (a * b) + c;
}

/// multiply add: a * b + c
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T multiply_add( T const &a, T const &b, T const &c ) {
    T r;
    for (size_t i = 0; i < r.size(); ++i) {
        r[i] = multiply_add(a[i],b[i],c[i]);
    }
    return r;
}

/// negative multiply subtract: - (a * b) - c
template <typename T, typename std::enable_if< is_not_simd<T>::value, bool>::type sfinae=true >
T negative_multiply_subtract( T const &a, T const &b, T const &c ) {
    return -((a*b) - c);
}

/// negative multiply subtract: - (a * b) - c
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T negative_multiply_subtract( T const &a, T const &b, T &c ) {
    T r;
    for (size_t i = 0; i < r.size(); ++i) {
        r[i] = negative_multiply_subtract(a[i],b[i],c[i]);
    }
    return r;
}


/** \addtogroup simd_sin sin */
/**@{*/
inline float simd_sin( float a ) {
    return std::sin(a);
}

inline double simd_sin( double a ) {
    return std::sin(a);
}

template <typename T>
inline std::complex<T> simd_sin( std::complex<T> a ) {
    return std::sin(a);
}

/// calculate sin() of each element
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T simd_sin( T const &a ) {
    T r;
    for( size_t i=0; i<a.size(); ++i ) {
        r[i] = simd_sin(a[i]);
    }
    return r;
}

/** \addtogroup simd_cos cos */
/**@{*/
inline float simd_cos( float a ) {
    return std::cos(a);
}

inline double simd_cos( double a ) {
    return std::cos(a);
}

template <typename T>
inline std::complex<T> simd_cos( std::complex<T> a ) {
    return std::cos(a);
}

/// calculate cos() of each element
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T simd_cos( T const &a ) {
    T r;
    for( size_t i=0; i<a.size(); ++i ) {
        r[i] = simd_cos(a[i]);
    }
    return r;
}

/**@}*/
/** \addtogroup simd_abs abs */
/**@{*/
inline float simd_abs( float a ) {
    return std::abs(a);
}

inline double simd_abs( double a ) {
    return std::abs(a);
}

template <typename T>
inline std::complex<T> simd_abs( std::complex<T> a ) {
    return std::abs(a);
}

/// calculate abs() of each element
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T simd_abs( T const &a ) {
    T r;
    for( size_t i=0; i<a.size(); ++i ) {
        r[i] = simd_abs(a[i]);
    }
    return r;
}

/**@}*/

/** \addtogroup simd_arg arg */
/**@{*/

inline float simd_arg( float a ) {
    return std::arg(a);
}

inline double simd_arg( double a ) {
    return std::arg(a);
}

template <typename T>
inline std::complex<T> simd_arg( std::complex<T> a ) {
    return std::arg(a);
}

/// calculate arg() of each element
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T simd_arg( T const &a ) {
    T r;
    for( size_t i=0; i<a.size(); ++i ) {
        r[i] = simd_arg(a[i]);
    }
    return r;
}
/**@}*/

inline float simd_sqrt( float a ) {
    return std::sqrtf(a);
}

inline double simd_sqrt( double a ) {
    return std::sqrt(a);
}

template <typename T>
inline std::complex<T> simd_sqrt( std::complex<T> a ) {
    return std::sqrt(a);
}


/// calculate sqrt() of each element
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T simd_sqrt( T const &a ) {
    T r;
    for( size_t i=0; i<a.size(); ++i ) {
        r[i] = simd_sqrt(a[i]);
    }
    return r;
}

/// calculate the reciprocal of each element
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T reciprocal( T const &a ) {
    T r;
    return apply(r,reciprocal,a);
}

/// calculate the reciprocal_estimate of each element
template <typename T, typename std::enable_if< is_simd<T>::value, bool>::type sfinae=true >
T reciprocal_estimate( T const &a ) {
    T r;
    return apply(r,reciprocal_estimate,a);
}


}
}
