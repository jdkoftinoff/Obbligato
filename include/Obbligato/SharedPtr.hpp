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
#include "Obbligato/Atomic.hpp"
#include "Obbligato/Deleter.hpp"
#include "Obbligato/IOStream.hpp"
#include <algorithm>

#include <memory>

namespace Obbligato
{
using std::shared_ptr;

#if __cplusplus >= 201103L
using std::make_shared;
using std::allocate_shared;
#else

template <typename T>
shared_ptr<T> make_shared()
{
    return shared_ptr<T>( new T );
}

template <typename T, typename Arg1T>
shared_ptr<T> make_shared( Arg1T &arg1 )
{
    return shared_ptr<T>( new T( arg1 ) );
}

template <typename T, typename Arg1T, typename Arg2T>
shared_ptr<T> make_shared( Arg1T &arg1, Arg2T &arg2 )
{
    return shared_ptr<T>( new T( arg1, arg2 ) );
}

template <typename T, typename Arg1T, typename Arg2T, typename Arg3T>
shared_ptr<T> make_shared( Arg1T &arg1, Arg2T &arg2, Arg3T &arg3 )
{
    return shared_ptr<T>( new T( arg1, arg2, arg3 ) );
}

template <typename T,
          typename Arg1T,
          typename Arg2T,
          typename Arg3T,
          typename Arg4T>
shared_ptr<T>
    make_shared( Arg1T &arg1, Arg2T &arg2, Arg3T &arg3, Arg4T &arg4 )
{
    return shared_ptr<T>( new T( arg1, arg2, arg3, arg4 ) );
}

template <typename T,
          typename Arg1T,
          typename Arg2T,
          typename Arg3T,
          typename Arg4T,
          typename Arg5T>
shared_ptr<T> make_shared(
    Arg1T &arg1, Arg2T &arg2, Arg3T &arg3, Arg4T &arg4, Arg5T &arg5 )
{
    return shared_ptr<T>( new T( arg1, arg2, arg3, arg4, arg5 ) );
}

template <typename T,
          typename Arg1T,
          typename Arg2T,
          typename Arg3T,
          typename Arg4T,
          typename Arg5T,
          typename Arg6T>
shared_ptr<T> make_shared( Arg1T &arg1,
                           Arg2T &arg2,
                           Arg3T &arg3,
                           Arg4T &arg4,
                           Arg5T &arg5,
                           Arg6T &arg6 )
{
    return shared_ptr<T>( new T( arg1, arg2, arg3, arg4, arg5 ) );
}

#endif

/// utility function to allow using operator << on shared pointer to
/// apply it on
/// the inner object
///
template <class E, class T, class Y>
inline std::basic_ostream<E, T> &
    operator<<( std::basic_ostream<E, T> &os, shared_ptr<Y> const &p )
{
    os << *p.get();
    return os;
}
}
