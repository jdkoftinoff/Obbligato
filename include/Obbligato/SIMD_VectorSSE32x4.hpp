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
#include "Obbligato/SIMD_Vector.hpp"

#if defined(__SSE__) && 0
#include "xmmintrin.h"

namespace Obbligato {
namespace SIMD {

template <> class OBBLIGATO_PLATFORM_VECTOR_ALIGN SIMD_Vector<float, 4> {
  public:
    typedef SIMD_Vector<float, 4> simd_type;
    typedef float value_type;
    enum {
        vector_size = 4
    };

    union {
        __m128 m_vec;
        value_type m_item[vector_size];
    };

    SIMD_Vector() {}

    SIMD_Vector( std::initializer_list<value_type> list ) {
        size_t n=0;
        for( value_type const *v=list.begin(); v!=list.end() && n<vector_size; ++v ) {
            m_item[n++] = *v;
        }
    }

    SIMD_Vector &set( std::initializer_list<value_type> list ) {
        size_t n=0;
        for( value_type const *v=list.begin(); v!=list.end() && n<vector_size; ++v ) {
            m_item[n++] = *v;
        }
        return *this;
    }

    SIMD_Vector &zero() {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] = value_type(0);
        }
        return *this;
    }

    SIMD_Vector &splat(value_type const &v) {
        for( size_t i=0; i<vector_size; ++i ) {
            m_item[i] = v;
        }
        return *this;
    }

    template <typename U> SIMD_Vector &set_item(U const &v, size_t index) {
        m_item[index] = v;
        return *this;
    }

    value_type const &get_item(size_t index) const { return m_item[index]; }

    value_type const &operator [](size_t index) const {
        return m_item[index];
    }

    value_type &operator[](size_t index) {
        return m_item[index];
    }


    SIMD_Vector const &operator=(SIMD_Vector const &other) {
        m_vec = other.m_vec;
        return *this;
    }


    SIMD_Vector const &operator+=(value_type p1) {
        simd_type p1v;
        p1v.splat( p1 );
        m_vec = _mm_add_ps(m_vec, p1v.m_vec);
        return *this;
    }

    SIMD_Vector const &operator+=(SIMD_Vector const &other) {
        m_vec = _mm_add_ps(m_vec, other.m_vec);
        return *this;
    }

    SIMD_Vector const &operator-=(value_type p1) {
        simd_type p1v;
        p1v.splat( p1 );
        m_vec = _mm_sub_ps(m_vec, p1v.m_vec);
        return *this;
    }

    SIMD_Vector const &operator-=(SIMD_Vector const &other) {
        m_vec = _mm_sub_ps(m_vec, other.m_vec);
        return *this;
    }

    SIMD_Vector const &operator*=(value_type p1) {
        simd_type p1v;
        p1v.splat( p1 );
        m_vec = _mm_mul_ps(m_vec, p1v.m_vec);
        return *this;
    }

    SIMD_Vector const &operator*=(SIMD_Vector const &other) {
        m_vec = _mm_mul_ps(m_vec, other.m_vec);
        return *this;
    }

    SIMD_Vector const &operator/=(value_type p1) {
        simd_type p1_recip;
        p1_recip.splat( p1 );
        p1_recip = reciprocal(p1_recip);
        m_vec = _mm_mul_ps(m_vec, p1_recip.m_vec);
        return *this;
    }

    SIMD_Vector const &operator/=(SIMD_Vector const &other) {
        simd_type other_recip = reciprocal(other);
        m_vec = _mm_mul_ps(m_vec, other_recip.m_vec);
        return *this;
    }
};

inline SIMD_Vector<float, 4> operator-(SIMD_Vector<float, 4> const &a) {
    SIMD_Vector<float, 4> r;
    r.m_vec = _mm_sub_ps(_mm_set1_ps(0.0f), a.m_vec);
    return r;
}


}
}
#endif
