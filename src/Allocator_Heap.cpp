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
#include "Obbligato/Allocator_Heap.hpp"
#include "Obbligato/Logger.hpp"


namespace Obbligato { namespace Allocator {

/**< us_allocator_heap_rounding_type_size is just the size of the largest fundamental type on this system */
const size_t AllocatorHeap::rounding_type_size = sizeof ( AllocatorHeap::rounding_type );

/**< us_allocator_rounded_block_size is the block_t structure size rounded to the next largest rounding_type boundary. */
const size_t AllocatorHeap::rounded_block_size =
        ( ( sizeof ( AllocatorHeap::HeapBlock )
            + ( sizeof(rounding_type) - 1 ) )
          / sizeof(rounding_type) )
        * sizeof( rounding_type);

AllocatorHeap::AllocatorHeap( void *raw, size_t size )
{
    internal_init_raw( raw, size );
}

void AllocatorHeap::validate_block( AllocatorHeap::HeapBlock *block )
{
    if ( (uint64_t)block < 0x1000 && block!=0 )
    {
        ob_log_error( "block is bad" );
        throw std::runtime_error("Heap: block is bad");
    }
    if ( block )
    {
        if ( block->m_magic != heap_magic() )
        {
            ob_log_error( "block->m_magic is bad" );
            throw std::runtime_error("Heap: magic is bad");
        }
        if ((uint64_t)block->m_next < 0x1000 && (uint64_t)block->m_next !=0 )
        {
            ob_log_error( "block->m_next is bad" );
            throw std::runtime_error("Heap: next is bad");
        }
        if ( (uint64_t)block->m_prev < 0x1000 && (uint64_t)block->m_prev !=0 )
        {
            ob_log_error( "block->m_prev is bad" );
            throw std::runtime_error("Heap: prev is bad");
        }
        if ( ((uint64_t)block->m_prev !=0) && (block->m_prev->m_next!=block) )
        {
            ob_log_error( "block->m_prev->m_next is bad" );
            throw std::runtime_error("Heap: prev->next is bad");
        }
        if ( ((uint64_t)block->m_next !=0) && (block->m_next->m_prev!=block) )
        {
            ob_log_error( "block->m_next->m_prev is bad" );
            throw std::runtime_error("Heap: next->prev is bad");
        }
    }
}


/**< create initial empty block in memory buffer */
void AllocatorHeap::internal_init()
{
    /* clear all allocations */
    m_current_allocation_count = 0;
    /* the first block is at the beginning of the buffer */
    m_first = (HeapBlock *) ( m_memory_base );
    /* set the first block entries, it is the only block and it is empty */
    m_first->m_magic = heap_magic();
    m_first->m_next = 0;
    m_first->m_prev = 0;
    m_first->m_size = (size_t)(- (ssize_t)( m_size
                                            - rounded_block_size )); /* negative size means empty */
    /* the first empty block is also our last free block */
    m_last_free = m_first;
}

void AllocatorHeap::internal_init_raw( void *base, size_t size )
{
    m_size = round_down_types( size );
    m_memory_base = base;
    internal_init();
}

AllocatorHeap::~AllocatorHeap()
{
}

void * AllocatorHeap::alloc( ssize_t length, ssize_t count )
{
    heap_validate();
    return( internal_alloc( round_size( length*count ) ));
}

void * AllocatorHeap::realloc(
        void const *orig_ptr,
        ssize_t length,
        ssize_t count
        )
{
    void *new_ptr = alloc ( length, count );
    if ( new_ptr )
    {
        memcpy ( new_ptr, orig_ptr, round_size ( length * count ) );
        free ( orig_ptr );
    }
    return new_ptr;
}

void AllocatorHeap::free( void const *ptr )
{
    heap_validate();
    internal_free( ptr );
}


/**< clear all blocks in the memory buffer, empty it out */
void AllocatorHeap::reset()
{
    internal_init();
}

void *AllocatorHeap::internal_alloc ( size_t size )
{
    HeapBlock *cur = m_last_free;
    HeapBlock *usable = 0;
    void *ptr = 0;
    /* find a free memory block that is big enough */
    if ( !cur )
    {
        cur = m_first;
    }
    /* allow for allocation of 0 sized amounts */
    if ( size == 0 )
    {
        size = rounding_type_size;
    }
    /* round the size to the nearest larger alignment size */
    size = round_up_bytes ( size );
    do
    {
        if ( ( cur->m_size ) < 0 && (size_t)( - ( cur->m_size ) ) >= size )
        {
            /* found a free memory block big enough! */
            usable = cur;
            break;
        }
        cur = cur->m_next;
    }
    while ( cur != NULL );
    /* if cur is NULL, we couldn't find ANY free blocks big enough */
    if ( cur == NULL )
    {
        /* We need to pack all free memory. Iterate through all blocks and call pack() on them. */
        cur = m_first;
        while ( cur != NULL )
        {
            heap_pack ( cur );
            cur = cur->m_next;
        }
        /* now try find a free block that may be large enough */
        cur = m_first;
        if ( cur )
        {
            do
            {
                if ( ( cur->m_size ) < 0 && (size_t) ( - ( cur->m_size ) )
                     >= size )
                {
                    /* found a free memory block big enough! */
                    usable = cur;
                    break;
                }
                cur = cur->m_next;
            }
            while ( cur != NULL );
        }
        if ( cur == NULL )
        {
            return 0; /* no memory! return 0 */
        }
    }
    /*
               we have found a big enough block. Check to see if we should
               split this block into two blocks so we don't waste memory
               Only split the block if the block is more than128 bytes larger
               than the requested amount
            */
    usable->m_size = -usable->m_size; /* mark the block as not free. */
    m_current_allocation_count += usable->m_size;
    if ( (size_t) ( usable->m_size ) > ( size + 128 ) )
    {
        /* yes, we should split the block into two free blocks */
        size_t orig_size = usable->m_size;
        HeapBlock *orig_next = usable->m_next;
        /* Make the block perfectly sized */
        usable->m_size = (ssize_t) ( size );
        m_current_allocation_count -= ( orig_size - size );
        /* calculate the position of the next block */
        usable->m_next
                = ( HeapBlock * ) ( (uint64_t)( usable )
                                    + size + rounded_block_size );
        /* put the links in properly */
        usable->m_next->m_next = orig_next;
        usable->m_next->m_prev = usable;
        usable->m_next->m_magic = heap_magic();
        if ( orig_next )
            orig_next->m_prev = usable->m_next;
        /* figure out how big the left over block is */
        usable->m_next->m_size = - ( (ssize_t) ( orig_size )
                                     - (ssize_t) ( size )
                                     - (ssize_t) ( rounded_block_size ) );
        m_last_free = usable->m_next;
        validate_block( usable );
        validate_block( usable->m_next );
        validate_block( usable->m_prev );
    }
    ptr = (void * ) ( (uint64_t) ( usable )
                      + rounded_block_size );
    /* return the data section of the block to the caller */
    return ptr;
}

void AllocatorHeap::internal_free ( void const *ptr )
{
    heap_pack( m_first );
    /* only free non-null pointers */
    heap_validate();
    if ( ptr != 0 )
    {
        /*
                   subtract the MemBlock header size from the pointer given to us
                   to get the MemBlock header address
                */
        HeapBlock *block =
                (HeapBlock *) ( ((uint64_t) ptr) - rounded_block_size );
        if ( block->m_magic != heap_magic() )
        {
            ob_log_error( "bad magic" );
            throw std::runtime_error("Heap: bad magic");
        }
        validate_block( block );
        /* mark it as free by changing the size to negative size */
        if ( block->m_size > 0 )
        {
            ssize_t sz = block->m_size;
            m_current_allocation_count -= sz;
            block->m_size = -sz;
        }
        else
        {
            ob_log_error( "attempt to double free a memory heap block" );
        }
        /* call pack() to pack it and any free blocks before and after it */
        heap_pack ( block );
    }
}

void AllocatorHeap::heap_pack ( HeapBlock *block )
{
    HeapBlock *first = NULL;
    HeapBlock *cur = NULL;
    HeapBlock *last = NULL;
    cur = block;
    /* block had better be already free before calling this! */
    if (cur)
    {
        heap_validate();
        if ( ( cur->m_size ) < 0 )
        {
            /* search backwards for contiguous free blocks */
            while ( cur->m_prev != NULL && ( cur->m_prev->m_size ) < 0 )
            {
                validate_block( cur );
                cur = cur->m_prev;
            }
            first = cur;
            /* update the heaps last free block cache so it does not get messed up */
            m_last_free = first;
            /* search forward for contiguous free blocks */
            cur = block;
            while ( cur->m_next != NULL && ( cur->m_next->m_size ) < 0 )
            {
                validate_block( cur );
                cur = cur->m_next;
            }
            last = cur;
            /*
                     if there is not only one lonely block of free memory,
                     there is space to reclaim
                     */
            if ( first != last )
            {
                /*
                         calculate size of reclaimed space
                         It is the difference between the first block
                         address and the last block address, plus the
                         size of the last block.
                         This value is then the size of the whole
                         free area including the HeapBlock header on the
                         first block. We subtract this from our total
                         and set the first block size to the negative
                         of that, since it is a free block
                         */
                first->m_size = - ( ( (char * ) ( last )
                                      - ( char * ) ( first ) ) + ( - ( last->m_size ) ) );
                /* fix links */
                first->m_next = last->m_next;
                if ( last->m_next != NULL )
                {
                    last->m_next->m_prev = first;
                }
                validate_block( first );
                validate_block( first->m_next );
                validate_block( first->m_prev );
            }
        }
    }
}

void AllocatorHeap::heap_report()
{
    uint32_t free_chunks = 0;
    uint32_t used_chunks = 0;
    ssize_t largest_free = 0;
    ssize_t largest_used = 0;
    size_t free_mem = 0;
    size_t used_mem = 0;
    HeapBlock *cur = m_first;
    ob_log_info( "AllocatorHeap dump" );
    while ( cur != NULL )
    {
        ssize_t sz = cur->m_size;
        ob_log_info( "heap block %p : next=%p, prev=%p, size=%d", cur, cur->m_next, cur->m_prev, cur->m_size );
        if ( sz < 0 )
        {
            free_mem += -sz;
            ++free_chunks;
            if ( -sz > largest_free )
                largest_free = -sz;
        }
        else
        {
            used_mem += sz;
            ++used_chunks;
            if ( sz > largest_used )
                largest_used = sz;
        }
        cur = cur->m_next;
    }
    ob_log_info( "size of largest free chunk: %d", largest_free );
    ob_log_info( "size of largest used chunk: %d ", largest_used );
    ob_log_info( "total free mem: %d ", free_mem );
    ob_log_info( "total used mem: %d", used_mem );
    ob_log_info( "free chunk count: %d", free_chunks );
    ob_log_info( "used chunk count: %d", used_chunks );
}

void AllocatorHeap::heap_validate()
{
    HeapBlock *cur = m_first;
    while ( cur != NULL )
    {
        validate_block( cur );
        cur = cur->m_next;
    }
}

}}


