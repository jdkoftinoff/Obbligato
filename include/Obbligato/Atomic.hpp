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
#include <atomic>

namespace Obbligato {
namespace Atomic {

template <typename AtomicValue, typename FunctorT>
inline AtomicValue atomic_modify(std::atomic<AtomicValue> &cur, FunctorT f) {
    AtomicValue old_val;
    AtomicValue new_val;

    do {
        old_val = cur.load();
        new_val = f(old_val);
    } while (!cur.compare_exchange_weak(old_val, new_val));

    return old_val;
}

template <class E, class T, class Y>
std::basic_ostream<E, T> &operator<<(std::basic_ostream<E, T> &os,
                                     std::atomic<Y> const &p) {
    Y v = p.load();
    os << v;
    return os;
}


template<typename T>
class LocklessQueue
{
public:

    struct LocklessNode {
        /**
         * @brief LocklessNode
         * @param value
         */
        LocklessNode(const T&& value)
          : m_value(std::move(value))
          , m_next(nullptr) {}

        T m_value;
        LocklessNode * m_next;
    };

    /**
     * @brief LocklessQueue
     */
    LocklessQueue()
      : m_back(nullptr) {}

    LocklessQueue( LocklessQueue const & ) = delete;
    LocklessQueue & operator = (LocklessQueue const & ) = delete;

    /**
     * @brief push
     * @param data
     */
    void push(const T &&data) {
        // Allocate a new LocklessNode and move the value into it
        LocklessNode * n = new LocklessNode(data);
        // set the next pointer in this new node to the current back of the list
        n->m_next = m_back.load(std::memory_order_relaxed);
        // Put this new node into place at the back of the list
        while(!std::atomic_compare_exchange_weak_explicit(
            &m_back,
            &n->next,
            n,
            std::memory_order_release,
            std::memory_order_relaxed)) {
            // if someone else beat us to it, then reset the next pointer and try again.
            n->m_next = m_back.load(std::memory_order_relaxed);
        }
    }

    /**
     * @brief pop_all
     * @return
     */
    LocklessNode * pop_all() {
        return m_back.exchange(nullptr, std::memory_order_consume);
    }
private:
    std::atomic<LocklessNode *> m_back;
};

}
}
