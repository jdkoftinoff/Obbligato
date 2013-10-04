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

#if defined(__AVX__) && 0
#include "immintrin.h"

namespace Obbligato {
namespace SIMD {
template <> class OBBLIGATO_PLATFORM_VECTOR_ALIGN SIMD_Vector<double, 4> {
  public:
    typedef SIMD_Vector<double, 4> simd_type;
    typedef double value_type;
    enum {
        vector_size = 4
    };

    union {
        __m256d m_vec;
        value_type m_item[vector_size];
    };

    SIMD_Vector() { zero(); }

    SIMD_Vector( std::initializer_list<T> list ) {
        size_t n=0;
        for( auto const &v=list.begin(); v!=list.end() && n<vector_size; ++v ) {
            m_item[n++] = v;
        }
    }

    SIMD_Vector &set( std::initializer_list<T> list ) {
        size_t n=0;
        for( auto const &v=list.begin(); v!=list.end() && n<vector_size; ++v ) {
            m_item[n++] = v;
        }
        return *this;
    }

    SIMD_Vector &zero() {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] = T(0);
        }
        return *this;
    }

    template <typename U> SIMD_Vector set_item(U const &v, size_t index) {
        m_item[index] = v;
        return *this;
    }

    value_type const &get_item(size_t index) const { return m_item[index]; }


    template <typename FuncT> void apply(FuncT f) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] = f(m_item[i]);
        }
    }

    template <typename FuncT> void apply(FuncT f, SIMD_Vector const &a) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] = f(a.m_item[i]);
        }
    }

    template <typename FuncT>
    void apply(FuncT f, SIMD_Vector const &a, SIMD_Vector const &b) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] = f(a.m_item[i], b.m_item[i]);
        }
    }

    template <typename FuncT>
    void apply(FuncT f, SIMD_Vector const &a, SIMD_Vector const &b,
               SIMD_Vector const &c) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] = f(a.m_item[i], b.m_item[i], c.m_item[i]);
        }
    }

    template <typename FuncT>
    void apply(FuncT f, SIMD_Vector const &a, SIMD_Vector const &b,
               SIMD_Vector const &c, SIMD_Vector const &d) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] = f(a.m_item[i], b.m_item[i], c.m_item[i], d.m_item[i]);
        }
    }

    SIMD_Vector const &operator=(SIMD_Vector const &other) {
        m_vec = other.m_vec;
        return *this;
    }

    SIMD_Vector const &operator=(value_type p1) {
        m_vec = _mm256_set1_pd(p1);
        return *this;
    }

    SIMD_Vector const &operator+=(value_type p1) {
        SIMD_Vector<double, 4> p1v = p1;
        m_vec = _mm256_add_pd(m_vec, p1v.m_vec);
        return *this;
    }

    SIMD_Vector const &operator+=(SIMD_Vector const &other) {
        m_vec = _mm256_add_pd(m_vec, other.m_vec);
        return *this;
    }

    SIMD_Vector const &operator-=(value_type p1) {
        SIMD_Vector<double, 4> p1v = p1;
        m_vec = _mm256_sub_pd(m_vec, p1v.m_vec);
        return *this;
    }

    SIMD_Vector const &operator-=(SIMD_Vector const &other) {
        m_vec = _mm256_sub_pd(m_vec, other.m_vec);
        return *this;
    }

    SIMD_Vector const &operator*=(value_type p1) {
        SIMD_Vector<double, 4> p1v = p1;
        m_vec = _mm256_mul_pd(m_vec, p1v.m_vec);
        return *this;
    }

    SIMD_Vector const &operator*=(SIMD_Vector const &other) {
        m_vec = _mm256_mul_pd(m_vec, other.m_vec);
        return *this;
    }

    SIMD_Vector const &operator/=(value_type p1) {
        SIMD_Vector<double, 4> p1_recip = p1;
        p1_recip = reciprocal(p1_recip);
        m_vec = _mm256_mul_pd(m_vec, p1_recip.m_vec);
        return *this;
    }

    SIMD_Vector const &operator/=(SIMD_Vector const &other) {
        SIMD_Vector<double, 4> other_recip = reciprocal(other);
        m_vec = _mm256_mul_pd(m_vec, other_recip.m_vec);
        return *this;
    }
};

inline SIMD_Vector<double, 4> operator-(SIMD_Vector<double, 4> const &a) {
    SIMD_Vector<double, 4> r;
    r.m_vec = _mm256_sub_pd(_mm256_set1_pd(0.0), a.m_vec);
    return r;
}
}
}
#endif
