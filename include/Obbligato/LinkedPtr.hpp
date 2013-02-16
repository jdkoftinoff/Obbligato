#pragma once
#ifndef Obbligato_LinkedPtr_hpp
#define Obbligato_LinkedPtr_hpp

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
    template <typename T>
    class LinkedPtr
    {
    public:
        typedef T value_type;

    private:
        value_type * m_ptr;
        mutable const LinkedPtr *m_prev;
        mutable const LinkedPtr *m_next;

    public:

        explicit LinkedPtr(value_type * p)
            : m_ptr(p)
        {
            m_prev = this;
            m_next = this;
        }

        ~LinkedPtr()
        {
            release();
        }

        LinkedPtr(LinkedPtr const & r)
        {
            acquire(r);
        }

        LinkedPtr& operator=(LinkedPtr const & r)
        {
            if (this != &r)
            {
                release();
                acquire(r);
            }
            return *this;
        }

        template <typename Y>
        LinkedPtr( LinkedPtr<Y> const & r)
        {
            acquire(r);
        }

        template <typename Y>
        LinkedPtr& operator=( LinkedPtr<Y> const & r)
        {
            if (this != &r)
            {
                release();
                acquire(r);
            }
            return *this;
        }

        value_type & operator*()  const
        {
            return *m_ptr;
        }

        value_type * operator->() const
        {
            return m_ptr;
        }

        value_type * get() const
        {
            return m_ptr;
        }

        bool unique() const
        {
            return m_prev ? m_prev==this : true;
        }

        void acquire(LinkedPtr const  & r)
        {
            m_ptr = r.m_ptr;
            m_next = r.m_next;
            m_next->m_prev = this;
            m_prev = &r;
            r.m_next = this;
        }

        template <typename Y>
        void acquire(LinkedPtr<Y> const & r)
        {
            m_ptr = r.m_ptr;
            m_next = r.m_next;
            m_next->m_prev = this;
            m_prev = &r;
            r.m_next = this;
        }

        void release()
        {
            if (unique())
            {
                delete m_ptr;
            }
            else
            {
                m_prev->m_next = m_next;
                m_next->m_prev = m_prev;
                m_prev = m_next = 0;
            }
            m_ptr = 0;
        }
    };

    template <typename T>
    static inline LinkedPtr<T> make_linked_ptr( T *p )
    {
        return LinkedPtr<T>(p);
    }


    template <typename T>
    class LinkedPtrComparator
    {
    private:
        T const *m_ptr;

    public:
        LinkedPtrComparator( T const *ptr ) : m_ptr(ptr)
        {
        }

        bool operator () ( LinkedPtr<T> const &v ) const
        {
            return m_ptr == v.get();
        }
    };

    template <typename T>
    inline LinkedPtrComparator<T> linkedptr_predicate( T const *v )
    {
        return LinkedPtrComparator<T>(v);
    }

}

#endif


