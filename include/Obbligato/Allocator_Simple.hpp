#pragma once
#ifndef Obbligato_Allocator_Simple_hpp
#define Obbligato_Allocator_Simple_hpp

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

#include "Obbligato/Allocator_Base.hpp"

namespace Obbligato { namespace Allocator {

class Simple : public AllocatorBase
{
    uint8_t * m_base_ptr;
    size_t m_size;
    size_t m_pos;
    size_t m_recent_pos;

public:
    Simple( void * base_ptr, size_t size )
        : m_base_ptr( static_cast<uint8_t *>(base_ptr) ),
          m_size( size ),
          m_pos( 0 ),
          m_recent_pos( 0 )
    {
    }

    virtual ~Simple()
    {
    }

    virtual void reset()
    {
        m_pos = 0;
        m_recent_pos = 0;
    }

    virtual void * alloc( ssize_t length, ssize_t count )
    {
        size_t total=length*count;
        uint8_t *r = 0;

        if( total+m_pos <= m_size )
        {
            m_recent_pos = m_pos;
            r=&m_base_ptr[m_pos];
            m_pos += total;
        }
        return r;
    }

    virtual void * realloc( const void *orig_ptr, ssize_t new_length, ssize_t new_count )
    {
        void *r=0;
        uint8_t const *orig_ptr_uint8 = static_cast<uint8_t const *>(orig_ptr);

        // Allow realloc only on the most recent allocation
        if( size_t(orig_ptr_uint8-m_base_ptr) == m_recent_pos )
        {
            size_t new_total=new_length*new_count;
            if( new_total+m_recent_pos <= m_size )
            {
                r = &m_base_ptr[m_recent_pos];
                m_pos = m_recent_pos+new_total;
            }
        }
        return r;
    }

    virtual void free( const void *orig_ptr )
    {
        (void)orig_ptr;
        // we never free anything.
    }

    virtual size_t total_available() const
    {
        return m_size-m_pos;
    }

    virtual size_t largest_available() const
    {
        return m_size-m_pos;
    }

};

}}

#endif


