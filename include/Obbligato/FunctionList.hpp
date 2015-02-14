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

#include "Obbligato/World.hpp"
#include "Obbligato/Atomic.hpp"

#if __cplusplus >= 201103L

namespace Obligatto
{
typedef uint32_t FunctionId;

class FunctionListBase
{
  public:
    FunctionId nextId() { return m_nextFunctionId++; }

    virtual void remove( FunctionId fid ) = 0;

  private:
    std::atomic<FunctionId> m_nextFunctionId;
};

template <typename F>
class FunctionList : public FunctionListBase
{
  public:
    using function_type = std::function<F>;
    using item_type = std::pair<FunctionId, function_type>;

    template <typename... Args>
    void operator()( Args... args ) const
    {
        for ( auto &i : m_functions )
        {
            i.second( args... );
        }
    }

    FunctionId add( function_type f )
    {
        FunctionId fid = nextId();
        m_functions.emplace_back( make_pair( fid, f ) );
        return fid;
    }

    virtual void remove( FunctionId fid )
    {
        m_functions.erase( std::remove_if( m_functions.begin(),
                                           m_functions.end(),
                                           [&]( item_type const &item )
                                           {
                               return item.first == fid;
                           } ),
                           m_functions.end() );
    }

    std::vector<item_type> m_functions;

    template <typename DestinationContainerT>
    void appendTo( DestinationContainerT &destination ) const
    {
        for ( auto i = m_functions.begin(); i != m_functions.end();
              ++i )
        {
            destination.push_back( i->second );
        }
    }

    class Registrar
    {
      public:
        Registrar( FunctionList &functionList, function_type func )
            : m_functionList( functionList )
            , m_fid( m_functionList.add( func ) )
        {
        }

        ~Registrar() { m_functionList.remove( m_fid ); }

        FunctionList &m_functionList;
        FunctionId m_fid;
    };
};

template <typename... ArgsT>
inline void invokeAll(
    std::vector<std::function<void( ArgsT... args )> > const &
        function_list,
    ArgsT &&... args )
{
    for ( auto i = function_list.begin(); i != function_list.end();
          ++i )
    {
        ( *i )( args... );
    }
}

template <typename... ArgsT>
inline void
    invokeAll( std::vector<FunctionList<void( ArgsT... args )> > const &
                   function_list,
               ArgsT &&... args )
{
    for ( auto i = function_list.m_functions.begin();
          i != function_list.m_functions.end();
          ++i )
    {
        ( *i )( args... );
    }
}

template <typename ResultT, typename... ArgsT>
inline std::vector<ResultT> invokeAll(
    std::vector<std::function<ResultT( ArgsT... args )> > const &
        function_list,
    ArgsT &&... args )
{
    std::vector<ResultT> results;
    for ( auto i = function_list.begin(); i != function_list.end();
          ++i )
    {
        results.push_back( ( *i )( args... ) );
    }
    return results;
}

template <typename ResultT, typename... ArgsT>
inline std::vector<ResultT> invokeAll(
    FunctionList<ResultT( ArgsT... args )> const &function_list,
    ArgsT &&... args )
{
    std::vector<ResultT> results;
    for ( auto i = function_list.m_functions.begin();
          i != function_list.m_functions.end();
          ++i )
    {
        results.push_back( ( *i )( args... ) );
    }
    return results;
}
}

#endif
