/*
 Copyright (c) 2013, J.D. Koftinoff Software, Ltd.
 <jeffk@jdkoftinoff.com>
 http://www.jdkoftinoff.com/
 All rights reserved.

 Permission to use, copy, modify, and/or distribute this software for
 any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.

 THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 WARRANTIES
 WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "Obbligato/World.hpp"
#include "Obbligato/Pool.hpp"
#include "Obbligato/Pools.hpp"
#include "Obbligato/PoolsAllocator.hpp"

namespace Obbligato
{

Pools::Pools( const char *name,
              void *( *low_level_allocation_function )( size_t ),
              void ( *low_level_free_function )( void * ) )
{
    m_name = name;
    m_low_level_allocation_function = low_level_allocation_function;
    m_low_level_free_function = low_level_free_function;
    m_diag_num_frees_from_heap = 0;
    m_diag_num_spills_handled = 0;
    m_diag_num_spills_to_heap = 0;
    m_num_pools = 0;
}

bool Pools::add( size_t element_size, size_t number_of_elements )
{
    bool r = false;
    if ( m_num_pools < OBBLIGATO_POOLS_MAX_POOLS )
    {
        pool[m_num_pools] = new Pool( number_of_elements,
                                      element_size,
                                      m_low_level_allocation_function,
                                      m_low_level_free_function );

        ++m_num_pools;
        r = true;
    }
    return r;
}

Pools::~Pools()
{
    size_t n;
    for ( n = 0; n < m_num_pools; ++n )
    {
        delete pool[n];
    }
    m_low_level_allocation_function = 0;
    m_low_level_free_function = 0;
}

void *Pools::allocateElement( size_t size )
{
    void *r = 0;
    size_t i;
    for ( i = 0; i < m_num_pools; ++i )
    {
        if ( size <= pool[i]->getElementSize() )
        {
            r = pool[i]->allocateElement();
            if ( r != 0 )
            {
                break;
            }
            else
            {
                ++m_diag_num_spills_handled;
            }
        }
    }
    if ( r == 0 && m_low_level_allocation_function )
    {
        ++m_diag_num_spills_to_heap;
        r = m_low_level_allocation_function( size );
    }
    return r;
}

void Pools::deallocateElement( void *p )
{
    size_t i;
    bool deallocated = false;
    if ( p )
    {
        for ( i = 0; i < m_num_pools; ++i )
        {
            if ( pool[i]->isAddressInPool( p ) )
            {
                pool[i]->deallocateElement( p );
                deallocated = true;
                break;
            }
        }
        if ( !deallocated && m_low_level_free_function )
        {
            ++m_diag_num_frees_from_heap;
            m_low_level_free_function( p );
        }
    }
}

void Pools::diagnostics( const char *prefix, std::ostream &o )
{
    size_t i;
    size_t total_items_still_allocated = 0;
    for ( i = 0; i < m_num_pools; ++i )
    {
        pool[i]->diagnostics( prefix, o );
        total_items_still_allocated
            += pool[i]->getTotalAllocatedItems();
    }

    o << prefix << ":summary:total_items_still_allocated :"
      << total_items_still_allocated << std::endl;
    o << prefix << ":summary:diag_num_frees_from_heap :"
      << m_diag_num_frees_from_heap << std::endl;
    o << prefix << ":summary:diag_num_spills_handled :"
      << m_diag_num_spills_handled << std::endl;
    o << prefix << ":summary:diag_num_spills_to_heap :"
      << m_diag_num_spills_to_heap << std::endl;
}
}
