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
#include "Obbligato/Atomic.hpp"
#include "Obbligato/Deleter.hpp"
#include "Obbligato/IOStream.hpp"
#include <algorithm>

#include <memory>

namespace Obbligato
{
using std::shared_ptr;
using std::make_shared;
using std::allocate_shared;

/// utility function to allow using operator << on shared pointer to apply it on
/// the inner object
///
template <class E, class T, class Y>
inline std::basic_ostream<E, T> &operator<<( std::basic_ostream<E, T> &os, shared_ptr<Y> const &p )
{
    os << *p.get();
    return os;
}

template <typename T, size_t N>
struct ReservedVector
{
    typedef typename std::vector<T>::size_type size_type;
    std::vector<T> m_vec;
    ReservedVector()
    {
        m_vec.reserve( N );
    }
    std::vector<T> const &operator*() const
    {
        return m_vec;
    }
    std::vector<T> &operator*()
    {
        return m_vec;
    }
    std::vector<T> const *operator->() const
    {
        return &m_vec;
    }
    std::vector<T> *operator->()
    {
        return &m_vec;
    }
};

template <typename T, size_t N, typename StorageT = ReservedVector<T, N + 1>>
struct ReservedQueue
{
    typedef typename StorageT::size_type size_type;
    typedef typename StorageT::const_iterator const_iterator;
    typedef typename StorageT::iterator iterator;
    typedef typename StorageT::const_pointer const_pointer;
    typedef typename StorageT::pointer pointer;
    typedef typename StorageT::const_reference const_reference;
    typedef typename StorageT::reference reference;

    StorageT m_storage;
    size_type m_next_in;
    size_type m_next_out;

    ReservedQueue() : m_next_in( 0 ), m_next_out( 0 )
    {
    }

    bool empty() const
    {
        return m_next_in == m_next_out;
    }

    size_type size() const
    {
        return m_storage.size();
    }

    size_type max_size() const
    {
        return m_storage.max_size() - 1;
    }

    size_type capacity() const
    {
        return m_storage.capacity() - 1;
    }

    void clear()
    {
        m_storage.clear();
        m_next_in = 0;
        m_next_out = 0;
    }

    const_reference front() const
    {
        return m_storage.front();
    }
    reference front()
    {
        return m_storage.front();
    }

    const_reference back() const
    {
        return m_storage.back();
    }
    reference back()
    {
        return m_storage.back();
    }

    const_reference at( size_type i ) const
    {
        return m_storage.at( i );
    }
    reference at( size_type i )
    {
        return m_storage.at( i );
    }

    void push_back( const T &v )
    {
        if ( size() >= capacity() )
        {
            throw std::bad_alloc();
        }
        m_storage[m_next_in] = v;
        m_next_in = ( m_next_in + 1 ) % ( m_storage.capacity() + 1 );
    }

    void push_back( T &&v )
    {
        if ( size() >= capacity() )
        {
            throw std::bad_alloc();
        }
        m_storage[m_next_in] = std::move( v );
        m_next_in = ( m_next_in + 1 ) % ( m_storage.capacity() + 1 );
    }

    template <class... Args>
    void emplace_back( Args &&... args )
    {
        if ( size() >= capacity() )
        {
            throw std::bad_alloc();
        }
        m_storage[m_next_in] = T( args... );
        m_next_in = ( m_next_in + 1 ) % ( m_storage.capacity() + 1 );
    }

    void pop_front()
    {
        m_next_out = ( m_next_out + 1 ) % ( m_storage.capacity() + 1 );
    }
};
}
