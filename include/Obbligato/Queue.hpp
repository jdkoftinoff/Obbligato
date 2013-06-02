#pragma once
#ifndef Obbligato_Queue_hpp
#define Obbligato_Queue_hpp

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

namespace Obbligato {

template <typename T>
class Queue
{
public:
    typedef T value_type;

    Queue( const Queue<T> &o )
        :
          m_max_items( o.m_max_items ),
          m_back_pos( o.m_back_pos ),
          m_front_pos( o.m_front_pos ),
          m_item_count( o.m_item_count ),
          m_items( new value_type[o.m_max_items])
    {
        for( size_t i=m_front_pos; i!=m_back_pos; i= ((i+1) % m_max_items) )
        {
            m_items[i] = o.m_items[i];
        }
    }

    Queue( size_t max_items )
        : m_max_items( max_items),
          m_back_pos(0),
          m_front_pos(0),
          m_item_count(0),
          m_items( new value_type[m_max_items] )
    {
    }

    ~Queue()
    {
        delete [] m_items;
    }

    const Queue<T> operator = ( const Queue<T> &o )
    {
        Queue<T> temp(o);
        temp.swap( *this );
        return *this;
    }

    void swap( Queue<T> &o )
    {
        std::swap( m_items, o.m_items );
        std::swap( m_max_items, o.m_max_items );
        std::swap( m_back_pos, o.m_back_pos);
        std::swap( m_front_pos, o.m_front_pos );
        std::swap( m_item_count, o.m_item_count );
    }

    void clear()
    {
        m_front_pos = m_back_pos;
        m_item_count=0;
    }

    bool full() const
    {
        return m_item_count == m_max_items;
    }

    bool empty() const
    {
        return m_item_count==0;
    }

    size_t size() const
    {
        return m_item_count;
    }

    size_t max_size() const
    {
        return m_max_items;
    }


    void push( value_type v )
    {
        if( m_item_count < m_max_items )
        {
            m_items[ m_back_pos ] = v;
            m_back_pos = (m_back_pos + 1)%m_max_items;
            ++m_item_count;
        }
        else
        {
            throw std::runtime_error("Queue::push() no space");
        }
    }

    void push( value_type const *buf, size_t count )
    {
        if( m_item_count + count <= m_max_items )
        {
            for( size_t i=0; i<count; ++i )
            {
                m_items[m_back_pos] = buf[i];
                m_back_pos=(m_back_pos+1)%m_max_items;
                m_item_count++;
            }
        }
        else
        {
            throw std::runtime_error("Queue::push() no space");
        }
    }

    template <size_t N>
    void push( value_type const buf[N] )
    {
        if( m_item_count + N < m_max_items )
        {
            for( size_t i=0; i<N; ++i )
            {
                m_items[m_back_pos] = buf[i];
                m_back_pos=(m_back_pos+1)%m_max_items;
                ++m_item_count;
            }
        }
        else
        {
            throw std::runtime_error("Queue::push() no space");
        }
    }

    void pop()
    {
        if( m_item_count>0 )
        {
            m_front_pos = (m_front_pos + 1) % m_max_items;
            --m_item_count;
        }
        else
        {
            throw std::runtime_error("Queue::pop() nothing to pop");
        }
    }

    void pop( size_t count )
    {
        for( size_t i=0; i<count && m_item_count>0; ++i )
        {
            pop();
        }
    }

    value_type front()
    {
        if( m_item_count==0 )
        {
            throw std::runtime_error("Queue::front() no front");
        }
        return m_items[m_front_pos];
    }

    const value_type front() const
    {
        if( m_item_count==0 )
        {
            throw std::runtime_error("Queue::front() no front");
        }
        return m_items[m_front_pos];
    }

    void front( value_type *buf, size_t count ) const
    {
        if( m_item_count<count)
        {
            throw std::runtime_error("Queue::front() not enough content");
        }
        for( size_t i=0; i<count; ++i )
        {
            buf[i] = m_items[(m_front_pos + i)%m_max_items];
        }
    }

    template <size_t N>
    void front( value_type buf[N]  ) const
    {
        if( m_item_count<N)
        {
            throw std::runtime_error("Queue::front() not enough content");
        }
        for( size_t i=0; i<N; ++i )
        {
            buf[i] = m_items[(m_front_pos + i)%m_max_items];
        }
    }


    value_type back()
    {
        if( m_item_count==0 )
        {
            throw std::runtime_error("Queue::back() no back");
        }
        return m_items[m_back_pos];
    }

    const value_type back() const
    {
        if( m_item_count==0 )
        {
            throw std::runtime_error("Queue::back() no back");
        }
        return m_items[m_front_pos];
    }
private:
    size_t m_max_items;
    size_t m_back_pos;
    size_t m_front_pos;
    Atomic::AtomicInt m_item_count;
    value_type *m_items;
};

}

#endif


