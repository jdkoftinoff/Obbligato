#pragma once
#ifndef Obbligato_FunctionList_hpp
#define Obbligato_FunctionList_hpp

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
#include "Obbligato/Function.hpp"

namespace Obbligato {

template <typename FunctionSignature> class FunctionList {};

template <> class FunctionList<VoidFunction> {
  public:
    typedef VoidFunction func_type;
    typedef std::list<VoidFunction> func_list_type;

    FunctionList() {}

    FunctionList(const FunctionList<func_type> &o) : m_list(o.m_list) {}

    FunctionList<func_type> &operator=(const FunctionList<func_type> &o) {
        m_list = o.m_list;
        return *this;
    }

    void push_back(VoidFunction proc) { m_list.push_back(proc); }

    void clear() { m_list.clear(); }

    void operator()() const {
        for (func_list_type::const_iterator i = m_list.begin();
             i != m_list.end(); ++i) {
            (*i)();
        }
    }

  private:

    func_list_type m_list;
};

typedef FunctionList<VoidFunction> VoidFunctionList;

#if defined(OBBLIGATO_CPLUSPLUS_HAS_VAR_TEMPLATE_ARGS)
template <typename... Args> class FunctionList<void(Args...)> {
  public:
    typedef function<void(Args...)> func_type;
    typedef std::list<func_type> func_list_type;

    FunctionList() {}

    FunctionList(func_list_type const &o) : m_list(o.m_list) {}

    func_list_type const &operator=(func_list_type const &o) {
        m_list = o.m_list;
        return *this;
    }

    void push_back(func_type proc) { m_list.push_back(proc); }

    void clear() { m_list.clear(); }

    void operator()(Args... args) const {
        for (typename func_list_type::const_iterator i = m_list.begin();
             i != m_list.end(); ++i) {
            (*i)(args...);
        }
    }

  private:

    func_list_type m_list;
};
#endif
}

#endif
