#pragma once
#ifndef Obbligato_ScopedPtr_hpp
#define Obbligato_ScopedPtr_hpp

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
#include "Obbligato/Noncopyable.hpp"
#include "Obbligato/Deleter.hpp"

namespace Obbligato {

/// Smart pointer which deletes when exiting scope
template <typename T >
class ScopedPtrBase : public Noncopyable
{
public:

    typedef T value_type;

private:
    value_type *m_ptr;
    DeleterBase<T> *m_deleter_ptr;

public:

    explicit ScopedPtrBase ( value_type *ptr, DeleterBase<T> *deleter_ptr )
        :
          m_ptr ( ptr ),
          m_deleter_ptr( deleter_ptr )
    {
    }

    ~ScopedPtrBase()
    {
        (*m_deleter_ptr)(m_ptr);
    }

    T & operator -> ()
    {
        return *m_ptr;
    }

    const T & operator -> () const
    {
        return *m_ptr;
    }

    T & get()
    {
        return *m_ptr;
    }

    const T & get() const
    {
        return *m_ptr;
    }
};


template <typename T, typename DeleterT=DefaultDeleter<T> >
class ScopedPtr : public ScopedPtrBase<T>
{
public:
    typedef T value_type;
    typedef DeleterT deleter_type;

private:
    DeleterT m_deleter;

public:

    explicit ScopedPtr ( T * ptr, DeleterT deleter=DeleterT() )
        : ScopedPtrBase<T>( ptr, &m_deleter ),
          m_deleter( deleter )
    {
    }
};

template <typename T>
inline ScopedPtr<T,DefaultDeleter<T> > make_scoped_ptr( T *ptr )
{
    return ScopedPtr<T,DefaultDeleter<T> >(ptr);
}

template <typename T, typename DeleterT >
inline ScopedPtr<T,DeleterT> make_scoped_ptr( T *ptr, DeleterT deleter=DeleterT() )
{
    return ScopedPtr<T,DeleterT>(ptr,deleter);
}


template <typename T, typename DeleterT=DefaultArrayDeleter<T> >
class ScopedArrayPtr : public ScopedPtrBase<T>
{
public:
    typedef T value_type;
    typedef DeleterT deleter_type;

private:
    DeleterT m_deleter;

public:

    explicit ScopedArrayPtr ( T * ptr, DeleterT deleter=DeleterT() )
        : ScopedPtrBase<T>( ptr, &m_deleter ),
          m_deleter( deleter )
    {
    }

    T & operator [] ( size_t n )
    {
        return this->get()[n];
    }

    T const & operator [] ( size_t n ) const
    {
        return this->get()[n];
    }

};

}

#endif


