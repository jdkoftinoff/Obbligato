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
#include "Obbligato/Deleter.hpp"
#include "Obbligato/SharedPtr.hpp"

namespace Obbligato {

/// fixed sized pre-allocated pool of objects, with built-in management
template <typename T> class Pool {
  private:

    /// Given a pointer to an item within the pool, figure out which item it is
    /// and call destroy_item() on it
    void destroy(void const *item) {
        uint8_t const *item_ptr = static_cast<uint8_t const *>(item);
        ptrdiff_t item_offset = item_ptr - m_items_storage.get();

        if (item_offset < 0 ||
            item_offset >= static_cast<ptrdiff_t>(sizeof(T) * m_num_items) ||
            (item_offset % sizeof(T)) != 0) {
            throw std::runtime_error("pool_t::destroy given bad pointer");
        }

        size_t item_number = item_offset / sizeof(T);
        destroy_item(item_number);
    }

    /// Given an item number in the pool, call the items destructor and mark the
    /// pool item as available.
    void destroy_item(size_t item) {
        if (m_allocated_flags[item]) {
            T *itemptr = &m_items[item];
            if (itemptr) {
                itemptr->~T();
            }
            m_allocated_flags[item] = false;
            /// This item will be re-used on the next allocation.
            m_next_available_slot_hint = item;
            --m_cur_allocated_count;
        } else {
            throw std::runtime_error(
                "pool_t::destroy_item with item that was not allocated");
        }
    }

    /// Find the next available slot and store the slot number as a hint for the
    /// next allocation.
    bool find_next_available_slot() {
        bool good = false;
        if (m_allocated_flags[m_next_available_slot_hint]) {
            for (size_t i = 1; i < m_num_items; ++i) {
                size_t n = i % m_num_items;
                if (m_allocated_flags[n] == false) {
                    m_next_available_slot_hint = n;
                    good = true;
                    break;
                }
            }
        } else {
            good = true;
        }
        return good;
    }

    /// Find a slot that is available, mark it as allocated and return a pointer
    /// to it.
    void *allocate() {
        void *r = 0;

        if (find_next_available_slot()) {
            T *itemptr = &m_items[m_next_available_slot_hint];
            r = itemptr;
            m_allocated_flags[m_next_available_slot_hint] = true;
            m_next_available_slot_hint =
                (m_next_available_slot_hint + 1) % m_num_items;
            ++m_cur_allocated_count;
        } else {
            throw std::bad_alloc();
        }

        return r;
    }

    /// Return true if all slots are allocated
    bool full() const { return m_cur_allocated_count == m_num_items; }

    /// The 'deleter' helper class which allows a smart pointer to do the right
    /// thing to notify the pool to delete the item
    class PoolDeleter : public DeleterBase<T> {
      public:
        Pool<T> *m_pool;

        PoolDeleter(Pool<T> *pool) : m_pool(pool) {}

        void operator()(T const *p) const { m_pool->destroy(p); }
    };

    friend class PoolDeleter;

    /// The number of items in the pool
    size_t m_num_items;

    /// The raw data storage for the items in the pool
    std::unique_ptr<uint8_t[]> m_items_storage;

    /// An alias of the storage area, cast as a T *
    T *m_items;

    /// The allocated flags for the items in the pool
    std::vector<bool> m_allocated_flags;

    /// The hint for the next available slot
    size_t m_next_available_slot_hint;

    /// The count of allocated slots
    size_t m_cur_allocated_count;

  public:

    /// Allocate a pool that contains num_items objects of type T
    Pool(size_t num_items)
        : m_num_items(num_items),
          m_items_storage(new uint8_t[sizeof(T) * num_items]),
          m_items(reinterpret_cast<T *>(m_items_storage)), m_allocated_flags(),
          m_next_available_slot_hint(0), m_cur_allocated_count(0) {
        m_allocated_flags.resize(num_items, false);
        for (size_t i = 0; i < m_num_items; ++i) {
            m_allocated_flags[i] = false;
        }
    }

    Pool(Pool &&other)
        : m_num_items(std::move(other.m_num_items)),
          m_items_storage(std::move(other.m_items_storage)),
          m_items(std::move(other.m_items)),
          m_allocated_flags(std::move(other.m_allocated_flags)),
          m_next_available_slot_hint(
              std::move(other.m_next_available_slot_hint)),
          m_cur_allocated_count(std::move(other.m_cur_allocated_count)) {}

    Pool const &operator=(Pool &&other) {
        m_num_items = std::move(other.m_num_items);
        m_items_storage = std::move(other.m_items_storage);
        m_items = std::move(other.m_items);
        m_allocated_flags = std::move(other.m_allocated_flags);
        m_next_available_slot_hint =
            std::move(other.m_next_available_slot_hint);
        m_cur_allocated_count = std::move(other.m_cur_allocated_count);
        return *this;
    }

    /// Destruct all allocated items in the pool
    ~Pool() {
        for (size_t i = 0; i < m_num_items; ++i) {
            if (m_allocated_flags[i]) {
                m_allocated_flags[i] = false;
                destroy_item(i);
            }
        }
    }

    /// Allocate an item from the pool and return a shared pointer to it
    shared_ptr<T> make_shared() {
        return shared_ptr<T>(new (allocate()) T(), PoolDeleter(this));
    }

    /// Allocate an item from the pool and return a shared pointer to it (var
    /// args)
    template <typename... Args> shared_ptr<T> make_shared(Args &&... args) {
        return shared_ptr<T>(new (allocate()) T(args...), Deleter(this));
    }
};
}
