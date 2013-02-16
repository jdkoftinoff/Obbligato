#pragma once
#ifndef Obbligato_Atomic_hpp
#define Obbligato_Atomic_hpp

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

#ifdef __APPLE__
#define OBBLIGATO_ATOMIC_APPLE
#include <libkern/OSAtomic.h>
#endif

#if defined(__GNUC__) && ( (__GNUC__ > 4) || ((__GNUC__ >= 4) && (__GNUC_MINOR__ >= 1)) )
#define OBBLIGATO_ATOMIC_NEWER_GNUC
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#define OBBLIGATO_ATOMIC_NEWER_MSC_VER
#endif

#if defined(_MSC_VER)
#define OBBLIGATO_ATOMIC_MSC_VER
#endif

#if defined(_WIN32)
#define OBBLIGATO_ATOMIC_WIN32
#endif

#if defined(_WIN32) && !defined(_MSC_VER)
#define OBBLIGATO_ATOMIC_NON_MSC_VER_WIN32
#endif

#if defined(_MSC_VER)
// \note: Must use /Oi option for VC++ to enable intrinsics
extern "C"
{
    void __cdecl _ReadWriteBarrier();
    LONG __cdecl _InterlockedCompareExchange ( LONG volatile* Dest,LONG Exchange, LONG Comp );
}
#endif


namespace Obbligato
{
    namespace Atomic
    {
        inline void memory_barrier()
        {
#if defined(OBBLIGATO_ATOMIC_APPLE)
            OSMemoryBarrier();
#elif defined(OBBLIGATO_ATOMIC_NEWER_MSC_VER)
            _ReadWriteBarrier();
#elif defined(OBBLIGATO_ATOMIC_NEWER_GNUC)
            __sync_synchronize();
#else
#   error "no memory barrier implemented for this platform"
#endif
        }
        
        template< class C, class D >
        inline bool compare_and_set ( volatile C * addr, D old, D nw )
        {
#if defined(OBBLIGATO_ATOMIC_APPLE)
            return OSAtomicCompareAndSwap32Barrier ( old,nw,addr );
#elif defined(OBBLIGATO_ATOMIC_MSC_VER)
            return _InterlockedCompareExchange ( addr,nw,old ) == old;
#elif defined(OBBLIGATO_ATOMIC_NON_MSC_VER_WIN32)
            return InterlockedCompareExchange ( addr,nw,old ) == old;
#elif defined(OBBLIGATO_ATOMIC_NEWER_GNUC)
            return __sync_bool_compare_and_swap ( addr, old, nw );
#else
#   error "no compare_and_set for this platform"
#endif
        }
        
#if defined(OBBLIGATO_ATOMIC_APPLE) || defined(OBBLIGATO_ATOMIC_WIN32)
        class AtomicInt
        {
        public:
            
#ifdef _WIN32
            typedef LONG type;
#else
            typedef int32_t type;
#endif
            
            AtomicInt ( type v = 0 )
            {
                *this = v;
            }
            
            operator type ( void ) const
            {
                memory_barrier();
                return value;
            }
            
            void operator = ( type v )
            {
                value = v;
                memory_barrier();
            }
            
            /* prefix operator */
            type operator ++()
            {
                return *this += 1;
            }
            
            /* prefix operator */
            type operator --()
            {
                return *this -= 1;
            }
            
            type operator += ( int32_t v )
            {
                for ( ;; )
                {
                    volatile type oldv = value;
                    volatile type newv = oldv + v;
                    if ( ::Obbligato::Atomic::compare_and_set ( &value,oldv,newv ) )
                        return newv;
                }
            }
            
            type operator -= ( int32_t v )
            {
                for ( ;; )
                {
                    volatile type oldv = value;
                    volatile type newv = oldv - v;
                    if ( ::Obbligato::Atomic::compare_and_set ( &value,oldv,newv ) )
                        return newv;
                }
            }
            
            /* postfix operator */
            type operator ++ ( int )
            {
                for ( ;; )
                {
                    volatile type oldv = value;
                    if ( ::Obbligato::Atomic::compare_and_set ( &value,oldv,oldv+1 ) )
                        return oldv;
                }
            }
            
            /* postfix operator */
            type operator -- ( int )
            {
                for ( ;; )
                {
                    volatile type oldv = value;
                    if ( ::Obbligato::Atomic::compare_and_set ( &value,oldv,oldv-1 ) )
                        return oldv;
                }
            }
            
            bool compare_and_set ( type old_value, type new_value )
            {
                return ::Obbligato::Atomic::compare_and_set ( &value, old_value, new_value );
            }
            
        private:
            mutable volatile type value;
        };
        
#elif defined(OBBLIGATO_ATOMIC_NEWER_GNUC)
        class AtomicInt
        {
        public:
            typedef int32_t type;
            
            explicit AtomicInt ( type v = 0 ) :
            value ( v )
            {
            }
            
            operator type ( void ) const
            {
                return __sync_fetch_and_add ( &value, 0 );
            }
            
            void operator = ( type v )
            {
                value = v;
                __sync_synchronize();
            }
            
            int32_t operator += ( type v )
            {
                return __sync_add_and_fetch ( &value, v );
            }
            
            int32_t operator -= ( type v )
            {
                return __sync_sub_and_fetch ( &value, v );
            }
            
            /* prefix operator */
            type operator ++ ( void )
            {
                return __sync_add_and_fetch ( &value, 1 );
            }
            
            /* prefix operator */
            type operator -- ( void )
            {
                return __sync_sub_and_fetch ( &value, 1 );
            }
            
            /* postfix operator */
            type operator ++ ( int )
            {
                return __sync_fetch_and_add ( &value, 1 );
            }
            
            /* postfix operator */
            type operator -- ( int )
            {
                return __sync_fetch_and_sub ( &value, 1 );
            }
            
            bool compare_and_set ( type old_value, type new_value )
            {
                return __sync_bool_compare_and_swap ( &value, old_value, new_value );
            }
            
        private:
            mutable volatile type value;
        };
        
#elif defined(OBBLIGATO_ATOMIC_GLIBCXX)
        
        class AtomicInt
        {
        public:
            typedef _Atomic_word type;
            
            explicit AtomicInt ( type v = 0 ) :
            value ( v )
            {}
            
            operator type ( void ) const
            {
                return __gnu_cxx::__exchange_and_add ( &value, 0 );
            }
            
            void operator = ( type v )
            {
                value = v;
            }
            
            type operator += ( type v )
            {
                value __gnu_cxx::__exchange_and_add ( &value, v ) + v;
                return value;
            }
            
            type operator -= ( type v )
            {
                value = __gnu_cxx::__exchange_and_add ( &value, -v ) - v;
                return value;
            }
            
            /* prefix operator */
            type operator ++ ( void )
            {
                return operator+= ( 1 );
            }
            
            /* prefix operator */
            type operator -- ( void )
            {
                return operator-= ( 1 );
            }
            
            /* postfix operator */
            type operator ++ ( int )
            {
                return __gnu_cxx::__exchange_and_add ( &value, 1 );
            }
            
            /* postfix operator */
            type operator -- ( int )
            {
                return __gnu_cxx::__exchange_and_add ( &value, -1 );
            }
            
            bool compare_and_set ( type old_value, type new_value )
            {
                return OBBLIGATO::compare_and_set ( &value, old_value, new_value );
            }
        private:
            mutable volatile type value;
        };
        
#endif
        
        class AtomicIncFunctor
        {
        public:
            AtomicInt::type operator() ( const AtomicInt::type  &v ) const
            {
                return v + 1;
            }
        };
        
        
        class AtomicDecFunctor
        {
        public:
            AtomicInt::type operator() ( const AtomicInt::type  &v ) const
            {
                return v - 1;
            }
        };
        
        class AtomicXorFunctor
        {
            AtomicInt::type m_xor;
            
        public:
            AtomicXorFunctor ( AtomicInt::type x )
            :
            m_xor ( x )
            {
            }
            
            AtomicInt::type operator() ( const AtomicInt::type &v ) const
            {
                return v ^ m_xor;
            }
        };
        
        class AtomicIncMaskFunctor
        {
            AtomicInt::type m_mask;
            
        public:
            
            AtomicIncMaskFunctor ( AtomicInt::type mask )
            :
            m_mask ( mask )
            {
            }
            
            AtomicInt::type operator() ( AtomicInt::type v ) const
            {
                return ( v + 1 ) & m_mask;
            }
        };

        class AtomicIncModFunctor
        {
            AtomicInt::type m_mod;

        public:

            AtomicIncModFunctor ( AtomicInt::type mod )
                : m_mod( mod )
            {
            }

            AtomicInt::type operator() ( AtomicInt::type v ) const
            {
                return ( v + 1 ) % m_mod;
            }
        };

        
        template< typename FunctorT >
        inline AtomicInt::type atomic_modify ( AtomicInt &cur, FunctorT f )
        {
            AtomicInt::type old_val;
            AtomicInt::type new_val;
            
            do
            {
                old_val = cur;
                new_val = f ( old_val );
            }
            while ( !cur.compare_and_set( old_val, new_val ) );
            
            return old_val;
        }
    }
    
}

#endif


