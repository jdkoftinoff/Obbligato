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
#include "Obbligato/Pool.hpp"

namespace Obbligato
{

Pool::Pool( size_t num_elements,
            size_t element_size,
            void *( *low_level_allocation_function )( size_t ),
            void ( *low_level_free_function )( void * ) )
{
    bool r = true;
    /* one bit per element */
    size_t size_of_allocated_flags_in_bytes = ( num_elements + 7 ) / 8;
    m_element_size = element_size;
    m_num_elements = num_elements;
    m_next_available_hint = 0;
    m_diag_num_allocations = 0;
    m_diag_num_frees = 0;
    m_diag_num_spills = 0;
    m_total_allocated_items = 0;
    m_diag_multiple_allocation_errors = 0;
    m_diag_multiple_deallocation_errors = 0;
    m_element_storage_size = num_elements * element_size;
    m_low_level_allocation_function = low_level_allocation_function;
    m_low_level_free_function = low_level_free_function;

    if ( m_element_storage_size > 0 )
    {
        m_allocated_flags = (unsigned char *)low_level_allocation_function( size_of_allocated_flags_in_bytes );
        if ( m_allocated_flags )
        {
            memset( m_allocated_flags, 0, size_of_allocated_flags_in_bytes );
            m_element_storage = (unsigned char *)low_level_allocation_function( m_element_storage_size );
            if ( m_element_storage )
            {
                memset( m_element_storage, 0, m_element_storage_size );
                r = false;
            }
            else
            {
                low_level_free_function( m_allocated_flags );
            }
        }
    }
    else
    {
        r = false;
    }

    if ( r == false )
    {
        throw std::bad_alloc();
    }
}

Pool::~Pool()
{
    if ( m_element_storage )
    {
        m_low_level_free_function( m_element_storage );
    }
    if ( m_allocated_flags )
    {
        m_low_level_free_function( m_allocated_flags );
    }
}

void *Pool::allocateElement()
{
    void *r = 0;
    if ( m_num_elements > 0 )
    {
        ssize_t item = -1;
        item = findNextAvailableElement();

        if ( item != -1 )
        {
            markElementAllocated( item );
            r = getAddressForElement( item );
            ++m_diag_num_allocations;
        }
        else
        {
            ++m_diag_num_spills;
        }
    }
    return r;
}

ssize_t Pool::deallocateElement( void *p )
{
    if ( m_num_elements > 0 )
    {
        ssize_t item = getElementForAddress( p );
        if ( item >= 0 )
        {
            markElementAvailable( item );
            ++m_diag_num_frees;
        }
        return item;
    }
    else
    {
        return -1;
    }
}

bool Pool::isElementAvailable( size_t element_num )
{
    bool r = false;
    if ( element_num < m_num_elements )
    {
        size_t element_div_8 = element_num / 8;
        size_t element_mod_8 = element_num & 7;
        unsigned char bit = ( 1 << element_mod_8 );
        unsigned char flags = m_allocated_flags[element_div_8];

        if ( ( flags & bit ) == 0 )
        {
            r = true;
        }
    }
    else
    {
        throw std::logic_error( "isElementAvailable given invalid element_num" );
    }
    return r;
}

void Pool::markElementAllocated( size_t element_num )
{
    size_t element_div_8 = element_num / 8;
    size_t element_mod_8 = element_num & 7;
    unsigned char bit = ( 1 << element_mod_8 );
    unsigned char flags = m_allocated_flags[element_div_8];

    if ( ( flags & bit ) == bit )
    {
        m_diag_multiple_allocation_errors++;
        throw std::logic_error( "Multiple allocation" );
    }
    else
    {
        m_allocated_flags[element_div_8] = flags | bit;
        ++m_total_allocated_items;
        m_next_available_hint = ( m_next_available_hint + 1 ) % m_num_elements;
    }
}

void Pool::markElementAvailable( size_t element_num )
{
    size_t element_div_8 = element_num / 8;
    size_t element_mod_8 = element_num & 7;
    unsigned char bit = ( 1 << element_mod_8 );
    unsigned char mask_bit = ~bit;
    unsigned char flags = m_allocated_flags[element_div_8];

    if ( ( flags & bit ) == 0 )
    {
        m_diag_multiple_deallocation_errors++;
        throw std::logic_error( "Multiple deallocation" );
    }
    else
    {
        m_allocated_flags[element_div_8] = flags & mask_bit;
        --m_total_allocated_items;
        m_next_available_hint = element_num;
    }
}

void *Pool::getAddressForElement( size_t element_num )
{
    unsigned char *base = (unsigned char *)m_element_storage;
    void *r = 0;
    if ( element_num >= m_num_elements )
    {
        throw std::logic_error( "get_address_for_element out of range" );
    }
    r = base + ( element_num * m_element_size );
    return r;
}

bool Pool::isAddressInPool( void const *p )
{
    unsigned char *base = (unsigned char *)m_element_storage;
    unsigned char *top = base + m_element_storage_size;
    unsigned char const *pp = (unsigned char const *)p;
    bool r = false;

    if ( base <= pp && pp < top )
    {
        int offset = (int)( pp - base );
        if ( ( offset % m_element_size ) == 0 )
        {
            r = true;
        }
    }
    return r;
}

ssize_t Pool::getElementForAddress( void const *p )
{
    unsigned char *base = (unsigned char *)m_element_storage;
    unsigned char *top = base + m_element_storage_size;
    unsigned char const *pp = (unsigned char const *)p;
    ssize_t r = -1;
    if ( base <= pp && pp < top )
    {
        int offset = (int)( pp - base );
        if ( ( offset % m_element_size ) == 0 )
        {
            r = offset / m_element_size;
        }
    }
    return r;
}

ssize_t Pool::findNextAvailableElement()
{
    ssize_t r = -1;
    if ( m_element_storage_size > 0 )
    {
        if ( m_total_allocated_items < m_num_elements )
        {
            size_t pos = m_next_available_hint;
            for ( size_t i = 0; i < m_num_elements; ++i )
            {
                ssize_t item = ( pos + i ) % m_num_elements;
                if ( isElementAvailable( item ) )
                {
                    r = item;
                    m_next_available_hint = item;
                    break;
                }
            }
        }
        else
        {
            r = -1;
        }
    }
    return r;
}

void Pool::diagnostics( const char *prefix, std::ostream &o )
{
    size_t actual_allocated_items = 0;
    size_t i;
    for ( i = 0; i < m_num_elements; ++i )
    {
        if ( !isElementAvailable( i ) )
        {
            actual_allocated_items++;
        }
    }

    o << prefix << "m_element_size: " << m_element_size << std::endl;
    o << prefix << "m_num_elements: " << m_num_elements << std::endl;
    o << prefix << "m_total_allocated_items: " << m_total_allocated_items << std::endl;
    o << prefix << "actual_allocated_items: " << actual_allocated_items << std::endl;
    o << prefix << "m_diag_multiple_allocation_errors: " << m_diag_multiple_allocation_errors << std::endl;
    o << prefix << "m_diag_multiple_deallocation_errors: " << m_diag_multiple_deallocation_errors << std::endl;
    o << prefix << "m_diag_num_allocations: " << m_diag_num_allocations << std::endl;
    o << prefix << "m_diag_num_frees: " << m_diag_num_frees << std::endl;
    o << prefix << "m_diag_num_spills: " << m_diag_num_spills << std::endl;
}
}
