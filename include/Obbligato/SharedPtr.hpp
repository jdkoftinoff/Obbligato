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

#if OBBLIGATO_PLATFORM_HAS_TR1_MEMORY
#include <tr1/memory>

namespace Obbligato
{
    using std::tr1::shared_ptr;
    using std::tr1::make_shared;
    using std::tr1::allocate_shared;
}

#elif OBBLIGATO_PLATFORM_HAS_CXX11_MEMORY

#include <memory>

namespace Obbligato
{
    using std::shared_ptr;
    using std::make_shared;
    using std::allocate_shared;
}

#else

namespace Obbligato
{
    /// The base class for the part that holds the single atomic usage count for a group of SharedPtr's
    class SharedPtrCountBase
    {
    public:
        /// Construct with pointer and initialize count to 0
        SharedPtrCountBase( void * ptr )
            : m_ptr( ptr ),
              m_count(0)
        {
        }

        /// The destructor is virtual so it can be overriden by the class that does the work
        virtual ~SharedPtrCountBase()
        {
        }

        void *m_ptr;
        Atomic::AtomicInt m_count;
    };

    /// Subclass of the SharedPtrCountBase which understands the type information as well as the deleter type
    template <typename T,typename DeleterT >
    class SharedPtrCount : public SharedPtrCountBase
    {
    public:

        /// Construct a SharedPtrCount object with the associated pointer and deleter
        SharedPtrCount( T * ptr, DeleterT deleter = DeleterT() )
            : SharedPtrCountBase( ptr ),
              m_deleter( deleter )
        {
        }

        /// Destroy the SharedPtrCount object - If the pointer is valid then it calls the deleter on it
        ~SharedPtrCount()
        {
            if( m_ptr )
            {
                T *p = reinterpret_cast< T * >(m_ptr);
                m_deleter( p );
            }
        }

        DeleterT m_deleter;
    };

    /// The SharedPtr class, roughly based on tr1/boost with some additions.
    template <typename T>
    class shared_ptr
    {
    private:
        void incref()
        {
            if( !m_master )
            {
                throw std::runtime_error("SharedPtr corruption");
            }

            ++m_master->m_count;
        }

        void decref()
        {
            if( !m_master )
            {
                throw std::runtime_error("SharedPtr corruption");
            }

            if( --m_master->m_count==0 )
            {
                delete m_master;
                m_master=0;
                m_ptr=0;
            }
        }


    public:
        typedef T element_type;

        /// reset the pointer
        void reset()
        {
            shared_ptr().swap(*this);
        }

        /// Reset the pointer to refer to the new pointer if it is convertible to T *
        template <typename U >
        void reset( U *p )
        {
            shared_ptr(p).swap(*this);
        }

        /// Reset the pointer to refer to the new deleter and new pointer if it is convertible to T *
        template <typename U, typename DeleterT >
        void reset( U *p, DeleterT deleter=DeleterT() )
        {
            shared_ptr<T>(p,deleter).swap(*this);
        }

        /// Construct a new SharedPtr with null ptr
        shared_ptr()
        : m_master( new SharedPtrCount<T,DefaultDeleter<T> >(0) ),
              m_ptr( 0 )
        {
            incref();
        }

        /// Construct a new SharedPtr if it is convertible to T *
        template <typename U>
        shared_ptr( U *p )
            : m_master( new SharedPtrCount<T,DefaultDeleter<T> >( p, DefaultDeleter<T>() ) ),
              m_ptr( p )
        {
            incref();
        }

        /// Construct a new SharedPtr with deleter if the pointer is convertible to T *
        template <typename U, typename DeleterT >
        shared_ptr( U *p, DeleterT deleter )
            : m_master( new SharedPtrCount<T,DeleterT>( p, deleter ) ),
              m_ptr( p )
        {
            incref();
        }

        /// Copy constructor, allows copies from other SharedPtr's that contain pointers that are convertible to T *
        template <typename U>
        shared_ptr( shared_ptr<U> const &o  )
            : m_master( o.m_master ),
              m_ptr( o.m_ptr )
        {
            incref();
        }

        /// Assignment operator, allows copies from other SharedPtr's that contain pointers that are convertible to T *
        template <typename U>
        shared_ptr<T> const & operator = ( shared_ptr<U> const &o )
        {
            shared_ptr<T> tmp(o);
            tmp.swap(*this);
            return *this;
        }

        /// Destructor simply decrements the reference count
        ~shared_ptr()
        {
            decref();
        }

        /// Swap the master and ptr with the passed object
        template <typename U>
        void swap( shared_ptr<U> &o )
        {
            using namespace std;
            swap( m_master, o.m_master );
            swap( m_ptr, o.m_ptr );
        }

        /// return true if the pointer is valid
        operator bool() const
        {
            return m_ptr!=0;
        }

        /// return true if the reference count is 1
        bool unique() const
        {
            return m_master->m_count==1;
        }

        /// return the reference count
        uint32_t use_count() const
        {
            return m_master->m_count;
        }

        // Get the pointer
        T * get()
        {
            return m_ptr;
        }

        /// Get a const pointer
        T const * get() const
        {
            return m_ptr;
        }

        /// Get a reference
        T & operator *()
        {
            return *m_ptr;
        }

        /// Hey a const reference
        T const & operator *() const
        {
            return *m_ptr;
        }

        /// Operator ->
        T * operator ->()
        {
            return m_ptr;
        }

        /// const Operator ->
        T const * operator ->() const
        {
            return m_ptr;
        }

        mutable SharedPtrCountBase *m_master;
        T *m_ptr;
    };

    /// Extract the pointer to the deleter used for the pointer
    template<class D, class T>
    D * get_deleter(shared_ptr<T> const & p)
    {
        SharedPtrCount<T,D> *m = static_cast< SharedPtrCount<T,D> * >( p.m_master );
        return &m->m_deleter;
    }

    /// Test two SharedPtr's for equality
    template<class T, class U>
    inline bool operator==(shared_ptr<T> const & a, shared_ptr<U> const & b)
    {
        return a.get() ==b.get();
    }

    /// Test two SharedPtr's for inequality
    template<class T, class U>
    inline bool operator!=(shared_ptr<T> const & a, shared_ptr<U> const & b)
    {
        return a.get() != b.get();
    }

    /// Test to SharedPtr's for ordering
    template<class T, class U>
    inline bool operator<(shared_ptr<T> const & a, shared_ptr<U> const & b)
    {
        return a.get() < b.get();
    }

    /// utility function to extract the pointer from a const SharedPtr
    template<class T>
    inline T const * get_pointer(shared_ptr<T> const & p)
    {
        return p.get();
    }

    /// utility function to extract the pointer from a const SharedPtr
    template<class T>
    inline T * get_pointer(shared_ptr<T> & p)
    {
        return p.get();
    }

    /// utility function to create a new shared pointer by doing a static cast on the original
    template<class T, class U>
    inline shared_ptr<T> static_pointer_cast(shared_ptr<U> const & r)
    {
        (void)static_cast<T*>(r.get());
        return shared_ptr<T>(r);
    }

    /// utility function to create a new shared pointer by doing a static cast on the original
    template<class T, class U>
    shared_ptr<T> const_pointer_cast(shared_ptr<U> const & r)
    {
        (void)const_cast<T*>(r.get());
        return shared_ptr<T>(r);
    }

    /// utility function to create a new shared pointer by doing a dynamic cast on the original
    template<class T, class U>
    shared_ptr<T> dynamic_pointer_cast(shared_ptr<U> const & r)
    {
        if( dynamic_cast<T *>(r.get()))
        {
            return shared_ptr<T>( r );
        }
        else
        {
            return shared_ptr<T>( 0 );
        }
    }

    template <typename T, typename Alloc>
    inline shared_ptr<T> allocate_shared( Alloc const &alloc )
    {
        T *ptr = alloc.allocate(1);
        if( ptr )
        {
            return shared_ptr<T>( new(ptr) T() );
        }
        else
        {
            throw std::bad_alloc();
        }
    }
    
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
#endif

namespace Obbligato
{
    /// utility function to allow using operator << on shared pointer to apply it on the inner object
    template<class E, class T, class Y>
    std::basic_ostream<E, T> & operator<< (std::basic_ostream<E, T> & os, shared_ptr<Y> const & p)
    {
        os << *p.get();
        return os;
    }
}

#endif


