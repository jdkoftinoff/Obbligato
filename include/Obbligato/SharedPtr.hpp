#pragma once
#ifndef Obbligato_SharedPtr_hpp
#define Obbligato_SharedPtr_hpp

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
#include "Obbligato/Atomic.hpp"
#include "Obbligato/Deleter.hpp"
#include "Obbligato/IOStream.hpp"
#include <algorithm>

#if OBBLIGATO_PLATFORM_HAS_TR1_MEMORY
#include <tr1/memory>

namespace Obbligato {

using std::tr1::shared_ptr;

template <typename T>
inline shared_ptr<T> make_shared()
{
    return shared_ptr<T>(new T());
}


#if OBBLIGATO_PLATFORM_HAS_RVALUE_REFS && OBBLIGATO_PLATFORM_HAS_VARIADIC_TMPL

/// Allocate an item from the pool and return a shared pointer to it (var args)
template<typename T, typename... Args>
shared_ptr<T> make_shared( Args && ... args )
{
    return shared_ptr<T>( new( allocate() ) T(args...) );
}

#else

/// approximation of make_shared for precompliance compilers - one argument version
template <typename T, typename Arg1>
inline shared_ptr<T> make_shared(Arg1 const & arg1)
{
    return shared_ptr<T>(new T(arg1) );
}

/// approximation of make_shared for precompliance compilers - one argument version
template <typename T, typename Arg1>
inline shared_ptr<T> make_shared(Arg1 & arg1)
{
    return shared_ptr<T>(new T(arg1) );
}

/// approximation of make_shared for precompliance compilers - two argument version
template <typename T, typename Arg1, typename Arg2>
inline shared_ptr<T> make_shared(Arg1 & arg1, Arg2 & arg2)
{
    return shared_ptr<T>(new T(arg1, arg2) );
}

/// approximation of make_shared for precompliance compilers - two argument version
template <typename T, typename Arg1, typename Arg2>
inline shared_ptr<T> make_shared(Arg1 const & arg1, Arg2 const & arg2)
{
    return shared_ptr<T>(new T(arg1, arg2) );
}

/// approximation of make_shared for precompliance compilers - three argument version
template <typename T, typename Arg1, typename Arg2, typename Arg3>
inline shared_ptr<T> make_shared(Arg1 const & arg1, Arg2 const & arg2, Arg3 const & arg3)
{
    return shared_ptr<T>(new T(arg1, arg2,arg3) );
}

/// approximation of make_shared for precompliance compilers - four argument version
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
inline shared_ptr<T> make_shared(Arg1 const & arg1, Arg2 const & arg2, Arg3 const & arg3, Arg4 const & arg4 )
{
    return shared_ptr<T>(new T(arg1, arg2,arg3,arg4) );
}

/// approximation of make_shared for precompliance compilers - five argument version
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
inline shared_ptr<T> make_shared(Arg1 const & arg1, Arg2 const & arg2, Arg3 const & arg3, Arg4 const & arg4, Arg5 const & arg5 )
{
    return shared_ptr<T>(new T(arg1, arg2,arg3,arg4,arg5) );
}

/// approximation of make_shared for precompliance compilers - six argument version
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
inline shared_ptr<T> make_shared(Arg1 const & arg1, Arg2 const & arg2, Arg3 const & arg3, Arg4 const & arg4, Arg5 const & arg5, Arg6 const & arg6 )
{
    return shared_ptr<T>(new T(arg1, arg2,arg3,arg4,arg5,arg6) );
}
#endif
}

#elif OBBLIGATO_PLATFORM_HAS_CXX11_MEMORY

#include <memory>

namespace Obbligato {
using std::shared_ptr;
using std::make_shared;
using std::allocate_shared;
}

#else
# error missing std::shared_ptr and std::make_shared
#endif

namespace Obbligato {
/// utility function to allow using operator << on shared pointer to apply it on the inner object
template<class E, class T, class Y>
std::basic_ostream<E, T> & operator<< (std::basic_ostream<E, T> & os, shared_ptr<Y> const & p)
{
    os << *p.get();
    return os;
}
}

#endif


