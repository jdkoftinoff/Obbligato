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

#include "Pool.hpp"
#include "Pools.hpp"

namespace Obbligato
{
template <typename T>
struct PoolsAllocator : public std::allocator<T>
{
    typedef size_t size_type;
    typedef T *pointer;
    typedef const T *const_pointer;

    template <typename U>
    struct rebind
    {
        typedef PoolsAllocator<U> other;
    };

    pointer allocate( size_type n, const void *hint = 0 )
    {
        pointer p = 0;
        if ( m_pools )
        {
            p = static_cast<pointer>( m_pools->allocateElement(n * sizeof( T ) ) );
        }
        else
        {
            p = std::allocator<T>::allocate( n, hint );
        }
        return p;
    }

    void deallocate( pointer p, size_type n )
    {
        if ( m_pools )
        {
            m_pools->deallocateElement(p);
        }
        else
        {
            return std::allocator<T>::deallocate( p, n );
        }
    }

    PoolsAllocator( Pools *pools_to_use ) throw() : std::allocator<T>(), m_pools( pools_to_use ) {}

    PoolsAllocator( const PoolsAllocator &a ) throw() : std::allocator<T>( a ), m_pools( a.m_pools ) {}

    template <class U>
    PoolsAllocator( const PoolsAllocator<U> &a ) throw()
        : std::allocator<T>( a ), m_pools( a.m_pools )
    {
    }

    ~PoolsAllocator() throw() {}

    Pools *m_pools;
};
}

