#pragma once
/*
 Copyright (c) 2015, J.D. Koftinoff Software, Ltd.
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

namespace Obbligato
{

template <typename T>
class Required
{
  public:
    Required() : m_value( new T() ) {}

#if __cplusplus >= 201103L
    template <typename... ArgT>
    Required( ArgT &&... args )
        : m_value( new T( std::forward<ArgT>( args )... ) )
    {
    }
    Required( Required &&other ) : m_value( std::move( other.m_value ) )
    {
    }
    Required &operator=( Required &&other )
    {
        m_value = std::move( other.m_value );
        return *this;
    }
    explicit operator bool() const
    {
        bool r = false;
        if ( m_value )
        {
            r = true;
        }
        return r;
    }

#endif

    ~Required() {}
    Required( const Required &other ) : m_value( new T( *other ) ) {}
    Required &operator=( const Required &other )
    {
        m_value.reset( new T( *other ) );
        return *this;
    }

    T &operator*() { return *m_value; }

    const T &operator*() const { return *m_value; }

    T &operator->() { return *m_value; }

    const T &operator->() const { return *m_value; }

    T *get() { return m_value.get(); }

    const T *get() const { return m_value.get(); }

  private:
    std::unique_ptr<T> m_value;
};
}
