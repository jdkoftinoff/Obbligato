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

#if __cplusplus >= 201103L
using std::shared_ptr;
using std::make_shared;
using std::allocate_shared;
#else

template <typename T>
struct shared_ptr_default_deleter
{
    void operator()( T const *p ) const { delete p; }
};

class shared_ptr_count_base
{
  public:
    shared_ptr_count_base( void *ptr_ ) : ptr( ptr_ ), count( 0 ) {}
    virtual ~shared_ptr_count_base() {}

    void *ptr;
    uint32_t count;
};

template <typename T, typename DeleterT>
class shared_ptr_count : public shared_ptr_count_base
{
  public:
    shared_ptr_count( T *ptr_, DeleterT deleter_ = DeleterT() )
        : shared_ptr_count_base( ptr_ ), deleter( deleter_ )
    {
    }

    ~shared_ptr_count()
    {
        if ( ptr )
        {
            T *p = reinterpret_cast<T *>( ptr );
            deleter( p );
        }
    }

    DeleterT deleter;
};

template <typename T>
class shared_ptr
{
  private:
    void incref()
    {
        if ( !master )
        {
            throw std::runtime_error( "shared_ptr corruption" );
        }

        ++master->count;
    }

    void decref()
    {
        if ( !master )
        {
            throw std::runtime_error( "shared_ptr corruption" );
        }

        if ( --master->count == 0 )
        {
            delete master;
            master = 0;
            ptr = 0;
        }
    }

  public:
    typedef T element_type;

    void reset() { shared_ptr().swap( *this ); }

    template <typename U>
    void reset( U *p )
    {
        shared_ptr( p ).swap( *this );
    }

    template <typename U, typename DeleterT>
    void reset( U *p, DeleterT deleter_ = DeleterT() )
    {
        shared_ptr<T>( p, deleter_ ).swap( *this );
    }

    shared_ptr()
        : master(
              new shared_ptr_count<T, shared_ptr_default_deleter<T> >(
                  0 ) )
        , ptr( 0 )
    {
        incref();
    }

    shared_ptr( T *p )
        : master(
              new shared_ptr_count<T, shared_ptr_default_deleter<T> >(
                  p, shared_ptr_default_deleter<T>() ) )
        , ptr( p )
    {
        incref();
    }

    template <typename DeleterT>
    shared_ptr( T *p, DeleterT deleter_ )
        : master( new shared_ptr_count<T, DeleterT>( p, deleter_ ) )
        , ptr( p )
    {
        incref();
    }

    shared_ptr( shared_ptr<T> const &o )
        : master( o.master ), ptr( o.ptr )
    {
        incref();
    }

    template <typename U>
    shared_ptr( shared_ptr<U> const &o )
        : master( o.master ), ptr( o.ptr )
    {
        incref();
    }

    shared_ptr<T> const &operator=( shared_ptr<T> const &o )
    {
        shared_ptr<T> tmp( o );
        tmp.swap( *this );
        return *this;
    }

    ~shared_ptr() { decref(); }

    void swap( shared_ptr<T> &o )
    {
        std::swap( master, o.master );
        std::swap( ptr, o.ptr );
    }

    operator bool() const { return ptr != 0; }
    bool unique() const { return master->count == 1; }
    uint32_t use_count() const { return master->count; }

    T *get() { return ptr; }
    T const *get() const { return ptr; }

    T &operator*() { return *ptr; }
    T const &operator*() const { return *ptr; }

    T *operator->() { return ptr; }
    T const *operator->() const { return ptr; }

    mutable shared_ptr_count_base *master;
    T *ptr;
};

template <class D, class T>
D *get_deleter( shared_ptr<T> const &p )
{
    shared_ptr_count<T, D> *m
        = static_cast<shared_ptr_count<T, D> *>( p.master );
    return &m->deleter;
}

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
