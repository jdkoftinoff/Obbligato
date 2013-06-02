#pragma once
#ifndef Obbligato_Allocator_Heap_hpp
#define Obbligato_Allocator_Heap_hpp

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

class AllocatorHeap : public AllocatorBase
{
public:
    AllocatorHeap( void * base_ptr, size_t size );

    virtual ~AllocatorHeap();

    virtual void reset();

    virtual void * alloc( ssize_t length, ssize_t count );

    virtual void * realloc( const void *orig_ptr, ssize_t new_length, ssize_t new_count );

    virtual void free( const void *orig_ptr );

private:

    static uint32_t heap_magic()
    {
        return 0x3141592;
    }

    /// Doubly linked list for a single free or allocated block
    struct HeapBlock
    {
        uint32_t m_magic;

        /// next block in line, or null
        HeapBlock *m_next;

        //// previous block in line, or null
        HeapBlock *m_prev;

        //// size of this block payload in bytes, not including the block_t structure. Negative for free block.
        ssize_t m_size;
    };

    void internal_init();

    void internal_init_raw( void *base, size_t size );

    /// try allocate an arbitrary buffer
    void *internal_alloc ( size_t size );

    void internal_free ( void const *ptr );

    void heap_pack ( HeapBlock *block );

    void heap_report();

    void heap_validate();

    void validate_block( HeapBlock *block );

private:

    /**< the rounded down size of the memory buffer */
    size_t m_size;

    /**< the base address of the memory buffer */
    void *m_memory_base;

    /**< the total count of allocated memory */
    size_t m_current_allocation_count;

    /**< the first block in the memory buffer */
    HeapBlock *m_first;

    /**< the last known free block in the memory buffer */
    HeapBlock *m_last_free;

    typedef size_t rounding_type;

    static const size_t rounding_type_size;
    static const size_t rounded_block_size;

    /**< round down a byte size to the nearest smaller alignment */
    size_t round_down_types ( size_t a )
    {
        return a & ~ ( rounding_type_size - 1 );
    }

    /**<  round up a byte size to the nearest larger alignment */
    size_t round_up_bytes ( size_t a )
    {
        return ( ( a + rounding_type_size - 1 ) / rounding_type_size )
                * rounding_type_size;
    }

};

}}

#endif


