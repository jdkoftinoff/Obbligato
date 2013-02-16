#pragma once
#ifndef Obbligato_Container_SharedQueue_hpp
#define Obbligato_Container_SharedQueue_hpp

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
#include "Obbligato/SharedPtr.hpp"

namespace Obbligato
{
    template <typename T>
    class SharedQueue
    {
    public:
        typedef shared_ptr<T> value_type;
        
        SharedQueue( const SharedQueue<T> &o )
            : m_max_items( o.m_max_items ),
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
        
        SharedQueue( size_t max_items_ )
            : m_max_items( max_items_),
              m_back_pos(0),
              m_front_pos(0),
              m_item_count(0),
              m_items( new value_type[max_items_] )
        {
        }
        
        ~SharedQueue()
        {
            delete [] m_items;
        }
        
        const SharedQueue<T> operator = ( const SharedQueue<T> &o )
        {
            SharedQueue<T> temp(o);
            temp.swap( *this );
            return *this;
        }
        
        void swap( SharedQueue<T> &o )
        {
            std::swap( m_items, o.m_items );
            std::swap( m_max_items, o.m_max_items );
            std::swap( m_back_pos, o.m_back_pos);
            std::swap( m_front_pos, o.m_front_pos );
            std::swap( m_item_count, o.m_item_count );
        }
        
        void clear()
        {
            while( m_front_pos!=m_back_pos )
            {
                m_items[m_front_pos].reset();
                m_front_pos = (m_front_pos + 1 ) % m_max_items;
            }
            m_item_count=0;
        }
        
        
        bool empty() const
        {
            return m_item_count==0;
        }
        
        bool full() const
        {
            return m_item_count == m_max_items;
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
                std::cout << "SharedQueue::push() no space." << std::endl;
                m_items[ m_back_pos ] = v;
                m_back_pos = (m_back_pos + 1)%m_max_items;
                ++m_item_count;
            }
        }
        
        void pop()
        {
            if( m_item_count>0 )
            {
                m_items[m_front_pos].reset();
                m_front_pos = (m_front_pos + 1) % m_max_items;
                --m_item_count;
            }
            else
            {
                throw std::runtime_error("SharedQueue::pop() nothing to pop");
            }
        }
        
        value_type front()
        {
            if( m_item_count==0 )
            {
                throw std::runtime_error("SharedQueue::front() no front");
            }
            return m_items[m_front_pos];
        }
        
        const value_type front() const
        {
            if( m_item_count==0 )
            {
                throw std::runtime_error("SharedQueue::front() no front");
            }
            return m_items[m_front_pos];
        }
        
        value_type back()
        {
            if( m_item_count==0 )
            {
                throw std::runtime_error("SharedQueue::back() no back");
            }
            return m_items[m_back_pos];
        }
        
        const value_type back() const
        {
            if( m_item_count==0 )
            {
                throw std::runtime_error("SharedQueue::back() no back");
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


