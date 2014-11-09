#pragma once

/*
 Copyright (c) 2014, J.D. Koftinoff Software, Ltd.
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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

namespace Obbligato
{

class Pool
{
  public:
    /**
     * @brief Pool                          Initialize a Pool
     * @param num_elements                  The number of elements to
     * allocate. May be 0 to disable Pool.
     * @param element_size                  The size of each element in
     * bytes.May be 0 to disable Pool.
     * @param low_level_allocation_function Pointer to low level memory
     * allocation function
     * @param low_level_free_function       Pointer to low level memory
     * free function
     * @return                              -1 on error, 0 on success
     */
    Pool( size_t m_num_elements,
          size_t m_element_size,
          void *( *m_low_level_allocation_function )( size_t ),
          void ( *m_low_level_free_function )( void * ) );

    /**
     * @brief destructor                Terminate a Pool and deallocate
     * low level buffers
     */
    ~Pool();

    /**
     * @brief getElementSize            Get the pool's element_size
     * @return                          The element size
     */
    size_t getElementSize() const { return m_element_size; }

    /**
     * @brief getTotalAllocatedItems    Get the total number of
     * allocated items
     * @return                          The number of allocated items
     */
    size_t getTotalAllocatedItems() const
    {
        return m_total_allocated_items;
    }

    /**
     * @brief allocateElement           Allocate one element from the
     * pool
     * @param self                      The pool to allocate from
     * @return                          0 on failure or pointer to
     * allocated element
     */
    void *allocateElement();

    /**
     * @brief deallocateElement         Deallocate one element from the
     * pool
     * @param p                         The pointer to deallocate
     * @return                          -1 if the item is not allocated
     * from this pool, or the item index if positive
     */
    ssize_t deallocateElement( void *p );

    /**
     * @brief isElementAvailable        Check to see if a specific
     * element index is available
     * @param element_num               The element index to check
     * @return                          true if the element is
     * available, 0 otherwise.
     */
    bool isElementAvailable( size_t element_num );

    /**
     * @brief markElementAllocated          Mark the specified element
     * as allocated
     * @param element_num                   The element index to mark as
     * allocated
     */
    void markElementAllocated( size_t element_num );

    /**
     * @brief markElementAvailable          Mark the specified element
     * as available
     * @param element_num                   The element index to mark as
     * available
     */
    void markElementAvailable( size_t element_num );

    /**
     * @brief getAddressForElement          Calculate the memory address
     * of a specific element
     * @param element_num                   The element index to use
     * @return                              Pointer to the element, or 0
     * if the element_num is out of range
     */
    void *getAddressForElement( size_t element_num );

    /**
     * @brief isAddressInPool               Calculate if the specified
     * address points to an element in this Pool
     * @param p                             The pointer to check
     * @return                              true if the address points
     * to the beginning of an element inside this Pool, false
     * otherwise
     */
    bool isAddressInPool( void const *p );

    /**
     * @brief Pool_get_element_for_address  Calculate the element number
     * given a pointer
     * @param p                             The pointer to check
     * @return                              The element number, or -1 if
     * the pointer is not pointing to the beginning of an
     * element
     * in this Pool
     */
    ssize_t getElementForAddress( void const *p );

    /**
     * @brief findNextAvailableElement      Find the next available
     * element in the pool
     * @return                              The element number, or -1 if
     * the there is no element available
     */
    ssize_t findNextAvailableElement();

    /**
     * @brief diagnostics               Print pool diagnostics counters
     * @param prefix                    Pointer to cstring which will be
     * put in front of each line outputted
     * @param o                         Reference to std::ostream to
     * output text to
     */
    void diagnostics( const char *prefix, std::ostream &o );

  private:
    /**
     * @brief num_elements The number of elements in this pool
     */
    size_t m_num_elements;

    /**
     * @brief element_size The size in bytes of each element
     */
    size_t m_element_size;

    /**
     * @brief next_available_hint The best guess of the next available
     * element
     */
    size_t m_next_available_hint;

    /**
     * @brief element_storage_size The total size in bytes of the
     * element_storage buffer
     */
    size_t m_element_storage_size;

    /**
     * @brief total_allocated_items The total number of items that are
     * currently allocated
     */
    size_t m_total_allocated_items;

    /**
     * @brief allocated_flags The storage for the bit map of
     * allocated/deallocated flags. One bit per element
     */
    unsigned char *m_allocated_flags;

    /**
     * @brief element_storage The storage for all of the element
     */
    unsigned char *m_element_storage;

    /**
     * @brief diag_num_allocations Diagnostics counter for the number of
     * allocations
     */
    size_t m_diag_num_allocations;

    /**
     * @brief diag_num_frees Diagnostics counter for the number of frees
     */
    size_t m_diag_num_frees;

    /**
     * @brief diag_num_spills Diagnostics counter for the number of
     * spills : allocations that had to be pushed to a larger pool
     */
    size_t m_diag_num_spills;

    /**
     * @brief diag_multiple_allocation_errors Diagnostics counter for
     * the number of times an element was allocated more than
     * once at a time
     */
    size_t m_diag_multiple_allocation_errors;

    /**
     * @brief diag_multiple_deallocation_errors Diagnostics counter for
     * the number of times an element was deallocated more than
     * once at a time
     */
    size_t m_diag_multiple_deallocation_errors;

    /**
     * @brief low_level_allocation_function the pointer to the system's
     * low level allocation function
     */
    void *( *m_low_level_allocation_function )( size_t );

    /**
     * @brief low_level_free_function The pointer to the system's low
     * level free dunction
     */
    void ( *m_low_level_free_function )( void * );
};
}
