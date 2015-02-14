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

#if __cplusplus >= 201103L

namespace Obbligato
{

class Choice
{
    using id = size_t;

    template <typename T>
    struct type
    {
        static void id() {}
    };

    template <typename T>
    static id type_id()
    {
        return reinterpret_cast<id>( &type<T>::id );
    }

    template <typename T>
    using decay = typename std::decay<T>::type;

    template <typename T>
    using none =
        typename std::enable_if<!std::is_same<Choice, T>::value>::type;

    struct base
    {
        virtual ~base() {}
        virtual bool is( id ) const = 0;
        virtual base *copy() const = 0;
    } *p = nullptr;

    template <typename T>
    struct data : base, std::tuple<T>
    {
        using std::tuple<T>::tuple;

        T &get() & { return std::get<0>( *this ); }
        T const &get() const & { return std::get<0>( *this ); }

        bool is( id i ) const override { return i == type_id<T>(); }
        base *copy() const override { return new data{get()}; }
    };

    template <typename T>
    T &stat()
    {
        return static_cast<data<T> &>( *p ).get();
    }

    template <typename T>
    T const &stat() const
    {
        return static_cast<data<T> const &>( *p ).get();
    }

    template <typename T>
    T &dyn()
    {
        return dynamic_cast<data<T> &>( *p ).get();
    }

    template <typename T>
    T const &dyn() const
    {
        return dynamic_cast<data<T> const &>( *p ).get();
    }

  public:
    Choice() {}
    ~Choice() { delete p; }

    Choice( Choice &&s ) : p{s.p} { s.p = nullptr; }
    Choice( Choice const &s ) : p{s.p->copy()} {}

    template <typename T, typename U = decay<T>, typename = none<U> >
    Choice( T &&x )
        : p{new data<U>{std::forward<T>( x )}}
    {
    }

    Choice &operator=( Choice s )
    {
        swap( *this, s );
        return *this;
    }

    friend void swap( Choice &s, Choice &r ) { std::swap( s.p, r.p ); }

    void clear()
    {
        delete p;
        p = nullptr;
    }

    bool empty() const { return p; }

    template <typename T>
    bool is() const
    {
        return p ? p->is( type_id<T>() ) : false;
    }

    template <typename T>
    T &&_() &&
    {
        return std::move( stat<T>() );
    }
    template <typename T>
    T &_() &
    {
        return stat<T>();
    }
    template <typename T>
    T const &_() const &
    {
        return stat<T>();
    }

    template <typename T>
    T &&cast() &&
    {
        return std::move( dyn<T>() );
    }
    template <typename T>
    T &cast() &
    {
        return dyn<T>();
    }
    template <typename T>
    T const &cast() const &
    {
        return dyn<T>();
    }

    template <typename T>
    operator T &&() &&
    {
        return std::move( _<T>() );
    }
    template <typename T>
    operator T &() &
    {
        return _<T>();
    }
    template <typename T>
    operator T const &() const &
    {
        return _<T>();
    }
};
}

#endif
