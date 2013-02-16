#ifndef Obbligato_Stack_hpp
#define Obbligato_Stack_hpp

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
#include "Obbligato/Deleter.hpp"

namespace Obbligato
{
    /// A Stack of objects
    template <typename T>
    class Stack
    {
        /// The Stack items
        T *m_items;

        /// Stack size
        size_t m_max_size;

        /// Stack position
        size_t m_position;

    public:
        typedef T value_type;

        /// Construct a Stack, default is 128
        Stack( size_t sz=128 )
            : m_items( new T[sz] ),
              m_max_size(sz),
              m_position(0)
        {
            if( ((sz-1) & sz) !=0 )
            {
                delete [] m_items;
                throw std::invalid_argument("Invalid Stack Size, must be power of 2");
            }
        }

        /// Destroy the Stack
        ~Stack()
        {
            delete [] m_items;
        }

        /// Test if Stack is empty
        bool empty() const
        {
            return m_position == 0;
        }

        /// Test if Stack is full
        bool full() const
        {
            return m_position == max_size();
        }

        /// get how many items are in the Stack
        size_t size() const
        {
            return m_position;
        }

        /// get the max Stack size, ie: the number of active items it can hold, which is always one less than the m_max_size
        size_t max_size() const
        {
            return m_max_size;
        }

        /// Get a reference to the next incoming item to be filled in
        T & incoming()
        {
            return m_items[m_position];
        }

        /// Get a reference to a future item to be filled in
        T & incoming(size_t pos)
        {
            size_t offset = (m_position + pos);
            return m_items[offset];
        }

        /// Get a reference to the next outgoing item
        T & outgoing()
        {
            return m_items[m_position-1];
        }

        /// Get a const reference to the next outgoing item
        T const & outgoing() const
        {
            return m_items[m_position-1];
        }

        /// Peek at an object in the outgoing Stack
        T & outgoing( size_t pos )
        {
            size_t offset = (m_position - pos - 1);
            return m_items[offset];
        }

        /// Peek at an object in the outgoing Stack
        T const & outgoing( size_t pos ) const
        {
            size_t offset = (m_position - pos - 1);
            return m_items[offset];
        }

        /// Copy an object and push it onto the Stack. Throws std::logic_error if the Stack is full.
        void push_back( T const &v )
        {
            if( !full() )
            {
                incoming() = v;
                push(1);
            }
            else
            {
                throw std::logic_error( "Trying to push_back() into full Stack" );
            }
        }


        /// Push the incoming object onto the Stack
        void push(size_t num=1)
        {
            m_position+=num;
        }

        /// Pop the outgoing object off of the Stack
        void pop(size_t num=1)
        {
            m_position-=num;
        }

        /// Get the outgoing object off of the Stack and pop it off. Throws std::logic_error if the Stack is empty
        T pop_back()
        {
            T r=outgoing();
            if( !empty() )
            {
                pop(1);
                return r;
            }
            else
            {
                throw std::logic_error( "Trying to pop_front() from empty Stack" );
            }
        }

        /// Get the position value
        size_t position() const
        {
            return m_position;
        }
    };

}

#endif


