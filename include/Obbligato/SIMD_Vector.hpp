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
    typedef SIMD_Vector<T, N> simd_type;

    typedef T value_type;
    enum {
        vector_size = N
    };

    value_type m_item[vector_size];

    SIMD_Vector() {}

    SIMD_Vector(value_type const &p1) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] = p1;
        }
    }

    SIMD_Vector(value_type const &p1, value_type const &p2) {
        m_item[0] = p1;
        m_item[1] = p2;
    }

    SIMD_Vector(value_type const &p1, value_type const &p2,
                value_type const &p3, value_type const &p4) {
        m_item[0] = p1;
        m_item[1] = p2;
        m_item[2] = p3;
        m_item[3] = p4;
    }

    SIMD_Vector(value_type const &p1, value_type const &p2,
                value_type const &p3, value_type const &p4,
                value_type const &p5, value_type const &p6,
                value_type const &p7, value_type const &p8) {
        m_item[0] = p1;
        m_item[1] = p2;
        m_item[2] = p3;
        m_item[3] = p4;
        m_item[4] = p5;
        m_item[5] = p6;
        m_item[6] = p7;
        m_item[7] = p8;
    }

    SIMD_Vector(SIMD_Vector const &other) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] = other.m_item[i];
        }
    }

    void zero() {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] = T(0);
        }
    }

    template <typename U> void set(U const &v, size_t index) {
        m_item[index] = v;
    }

    value_type const &get(size_t index) const { return m_item[index]; }

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
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] = other.m_item[i];
        }
        return *this;
    }

    SIMD_Vector const &operator=(value_type const &p1) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] = p1;
        }
        return *this;
    }

    SIMD_Vector const &operator+=(value_type const &p1) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] += p1;
        }
        return *this;
    }

    SIMD_Vector const &operator+=(SIMD_Vector const &other) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] += other.m_item[i];
        }
        return *this;
    }

    SIMD_Vector const &operator-=(value_type const &p1) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] -= p1;
        }
        return *this;
    }

    SIMD_Vector const &operator-=(SIMD_Vector const &other) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] -= other.m_item[i];
        }
        return *this;
    }

    SIMD_Vector const &operator*=(value_type const &p1) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] *= p1;
        }
        return *this;
    }

    SIMD_Vector const &operator*=(SIMD_Vector const &other) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] *= other.m_item[i];
        }
        return *this;
    }

    SIMD_Vector const &operator/=(value_type const &p1) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] /= p1;
        }
        return *this;
    }

    SIMD_Vector const &operator/=(SIMD_Vector const &other) {
        for (size_t i = 0; i < vector_size; ++i) {
            m_item[i] /= other.m_item[i];
        }
        return *this;
    }
};

template <typename T> T get(T const &v, size_t pos = 0) {
    if (pos != 0) {
        throw std::range_error("pos!=0 for non-vector get");
    }
    return v;
}

template <typename T> typename T::simd_type::value_type get(T &v, size_t pos) {
    return v.get(pos);
}

template <typename T> void set(T &d, T &v, size_t pos = 0) {
    if (pos != 0) {
        throw std::range_error("pos!=0 for non-vector get");
    }
    d = v;
}

template <typename T>
void set(T &d, typename T::simd_type::value_type const &v, size_t pos) {
    d.set(v, pos);
}

template <typename CharT, typename TraitsT, typename T, size_t N>
inline std::basic_ostream<CharT, TraitsT> &operator<<(
    std::basic_ostream<CharT, TraitsT> &str, SIMD_Vector<T, N> const &a) {
    str << "{ ";
    for (size_t i = 0; i < N - 1; ++i) {
        str << a.get(i) << ", ";
    }
    str << a.get(N - 1) << " }";
    return str;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> operator<(SIMD_Vector<T, N> const &a,
                                   SIMD_Vector<T, N> const &b) {
    SIMD_Vector<T, N> r;
    for (size_t i = 0; i < N; ++i) {
        r.set(T(a.get(i) < b.get(i)), i);
    }
    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> operator<=(SIMD_Vector<T, N> const &a,
                                    SIMD_Vector<T, N> const &b) {
    SIMD_Vector<T, N> r;
    for (size_t i = 0; i < N; ++i) {
        r.set(T(a.get(i) <= b.get(i)), i);
    }
    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> operator>=(SIMD_Vector<T, N> const &a,
                                    SIMD_Vector<T, N> const &b) {
    SIMD_Vector<T, N> r;
    for (size_t i = 0; i < N; ++i) {
        r.set(T(a.get(i) >= b.get(i)), i);
    }
    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> operator!=(SIMD_Vector<T, N> const &a,
                                    SIMD_Vector<T, N> const &b) {
    SIMD_Vector<T, N> r;
    for (size_t i = 0; i < N; ++i) {
        r.set(T(a.get(i) != b.get(i)), i);
    }
    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> operator==(SIMD_Vector<T, N> const &a,
                                    SIMD_Vector<T, N> const &b) {
    SIMD_Vector<T, N> r;
    for (size_t i = 0; i < N; ++i) {
        r.set(T(a.get(i) == b.get(i)), i);
    }
    return r;
}

template <typename T, size_t N, typename FuncT>
inline SIMD_Vector<T, N> apply(FuncT f, SIMD_Vector<T, N> const &a) {
    SIMD_Vector<T, N> r = a;
    r.apply(f);
    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> operator-(SIMD_Vector<T, N> const &a) {
    SIMD_Vector<T, N> r;
    for (size_t i = 0; i < N; ++i) {
        r.m_item[i] = -a.m_item[i];
    }
    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> operator+(SIMD_Vector<T, N> const &a,
                                   SIMD_Vector<T, N> const &b) {
    SIMD_Vector<T, N> r = a;
    r += b;
    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> operator-(SIMD_Vector<T, N> const &a,
                                   SIMD_Vector<T, N> const &b) {
    SIMD_Vector<T, N> r = a;
    r -= b;
    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> operator*(SIMD_Vector<T, N> const &a,
                                   SIMD_Vector<T, N> const &b) {
    SIMD_Vector<T, N> r = a;
    r *= b;
    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> operator/(SIMD_Vector<T, N> const &a,
                                   SIMD_Vector<T, N> const &b) {
    SIMD_Vector<T, N> r = a;
    r /= b;
    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> sin(SIMD_Vector<T, N> const &a) {
    SIMD_Vector<T, N> r = a;
    r.apply(::sin);
    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> sinf(SIMD_Vector<T, N> const &a) {
    SIMD_Vector<T, N> r = a;
    r.apply(::sinf);
    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> cos(SIMD_Vector<T, N> const &a) {
    SIMD_Vector<T, N> r = a;
    r.apply(::cos);
    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> cosf(SIMD_Vector<T, N> const &a) {
    SIMD_Vector<T, N> r = a;
    r.apply(::cosf);
    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> fabs(SIMD_Vector<T, N> const &a) {
    SIMD_Vector<T, N> r = a;
    r.apply(::fabs);
    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> fabsf(SIMD_Vector<T, N> const &a) {
    SIMD_Vector<T, N> r = a;
    r.apply(::fabsf);
    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> sqrt(SIMD_Vector<T, N> const &a) {
    SIMD_Vector<T, N> r = a;
    r.apply(::sqrt);
    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> sqrtf(SIMD_Vector<T, N> const &a) {
    SIMD_Vector<T, N> r = a;
    r.apply(::sqrtf);
    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> reciprocal(SIMD_Vector<T, N> const &a) {
    SIMD_Vector<T, N> r = a;

    for (size_t i = 0; i < N; ++i) {
        r.set(T(1) / a.get(i), i);
    }

    return r;
}

template <typename T, size_t N>
inline SIMD_Vector<T, N> reciprocal_estimate(SIMD_Vector<T, N> const &a) {
    SIMD_Vector<T, N> r = a;

    for (size_t i = 0; i < N; ++i) {
        r.set(T(1) / a.get(i), i);
    }

    return r;
}
}
}
