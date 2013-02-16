#pragma once
#ifndef Obbligato_Allocator_Base_hpp
#define Obbligato_Allocator_Base_hpp

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

namespace Obbligato
{
    namespace Allocator
    {
        
        /// Base class for all Allocators
        class AllocatorBase : public Noncopyable
        {
        public:
            virtual ~AllocatorBase()
            {
            }
            
            /// Reset/force free all allocations
            virtual void reset() = 0;
            
            /// Allocate count items of specified length
            virtual void * alloc( ssize_t length, ssize_t count ) = 0;
            
            /// Try re-allocate a previously allocated buffer
            virtual void * realloc( const void *orig_ptr, ssize_t new_length, ssize_t new_count ) = 0;
            
            /// Free a previously allocated buffer
            virtual void free( const void *orig_ptr ) = 0;
            
            /// Calculate total available storage
            virtual size_t total_available() const = 0;
            
            /// Calculate largest available chunk
            virtual size_t largest_available() const = 0;

            /** us_round_size

                Calculate size in bytes rounded up to the nearest  word size.
            */

            static inline ssize_t round_size( ssize_t v )
            {
                return (((v) + (size_t)(sizeof (size_t))-1) & (size_t)~(sizeof (size_t)-1));
            }

        };
        
        
        template <typename T>
        struct Deleter : public DeleterBase<T>
        {
            AllocatorBase *m_allocator;
            
            Deleter( AllocatorBase *m_allocator ) : m_allocator(m_allocator)
            {
            }
            
            void operator () ( T *p ) const
            {
                p->~T();
                m_allocator->free(p);
            }
        };
    }
}


#endif


