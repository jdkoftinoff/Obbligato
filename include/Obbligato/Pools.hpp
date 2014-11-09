#pragma once

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

#define OBBLIGATO_POOLS_MAX_POOLS ( 16 )

namespace Obbligato
{

class Pools
{
public:

    Pools( const Pools & ) = delete;
    Pools & operator = ( const Pools & ) = delete;

    /**
     * @brief Constructor                   Initialize a Pools object, a set of POOLS_MAX_POOLS pools
     * @param name                          Pointer to string of name of this collection of pools
     * @param low_level_allocation_function Pointer to low level memory allocation function
     * @param low_level_free_function       Pointer to low level memory free function
     */
    Pools( const char *name,
                    void *( *low_level_allocation_function )( size_t ),
                    void ( *low_level_free_function )( void * ) );

    /**
     * @brief destructor           Terminate a Pools and deallocate low level buffers used by all
     *                             pools except the spills onto the heap.
     */
     ~Pools();


    /**
     * @brief add                     Add a pool to a set of Pools
     * @param element_size                  The size of the element for this new pool
     * @param num_elements                  The number of elements for this new pool
     * @return                              false on error, true on success
     */
    bool add( size_t element_size, size_t number_of_elements );

    /**
     * @brief allocate_element    Attempt to allocate space for an object from the best Pool, or use the heap if none are
     * available.
     * @param size                      Size of the item to allocate
     * @return                          pointer to allocated item, or 0 on error
     */
    void *allocateElement( size_t size );

    /**
     * @brief deallocate_element  Find the pool that a pointer was allocated from and do the appropriate thing to de-allocate
     * it.
     * @param p                         Pointer to allocated item
     */
    void deallocateElement( void *p );

    /**
     * @brief diagnostics          Print pool diagnostics counters
     * @param prefix                    Pointer to cstring which will be put in front of each line outputted
     */
    void diagnostics( const char *prefix, std::ostream &o );

private:

    /**
     * @brief m_num_pools The current number of pools
     */
    size_t m_num_pools;

    /**
     * @brief pool The array of pools.
     */
    Pool *pool[OBBLIGATO_POOLS_MAX_POOLS];

    /**
     * @brief low_level_allocation_function the pointer to the system's low level allocation function
     */
    void *( *m_low_level_allocation_function )( size_t );

    /**
     * @brief low_level_free_function The pointer to the system's low level free dunction
     */
    void ( *m_low_level_free_function )( void * );

    /**
     * @brief diag_num_spills_handled Diagnostics counter of the number of spills that happened but were handled by another Pool
     */
    size_t m_diag_num_spills_handled;

    /**
     * @brief diag_num_spills_to_heap Diagnostics counter of the number of spills that happened that had to be handled by the
     * system heap
     */
    size_t m_diag_num_spills_to_heap;

    /**
     * @brief diag_num_frees_from_heap Diagnostics counter of the number of frees of objects that were allocated on the heap
     * because of a spill
     */
    size_t m_diag_num_frees_from_heap;

    /**
     * @brief name The name of this collection of Pools
     */
    const char *m_name;
};


}


