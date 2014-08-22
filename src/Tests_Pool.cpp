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
#include "Obbligato/Tests_Pool.hpp"

namespace Obbligato
{
namespace Tests
{

namespace TestsPool
{

using namespace IOStream;

struct MyItem
{
    int m_v;
    MyItem() : m_v( 0 )
    {
        ob_log_debug( title_fmt( "MyItem()" ) << this );
    }

    MyItem( int v ) : m_v( v )
    {
        ob_log_debug( title_fmt( "MyItem(int)" ) << this );
    }

    MyItem( const MyItem &other ) : m_v( other.m_v )
    {
        ob_log_debug( title_fmt( "MyItem(const MyItem &)" ) << this );
    }

    MyItem( MyItem &&other ) : m_v( other.m_v )
    {
        ob_log_debug( title_fmt( "MyItem(MyItem &&)" ) << this );
    }

    ~MyItem()
    {
        ob_log_debug( title_fmt( "~MyItem()" ) << this );
    }

    MyItem &operator=( MyItem const &other )
    {
        ob_log_debug( title_fmt( "MyItem::operator = (MyItem const &)" ) << this );
        m_v = other.m_v;
        return *this;
    }
};
}

void dump( Pool<TestsPool::MyItem> const &p, std::vector<std::shared_ptr<TestsPool::MyItem>> const &v )
{
    using namespace IOStream;

    ob_log_debug( title_fmt( "test_pool" ) << &p );
    ob_log_debug( label_fmt( "capacity" ) << p.capacity() );
    ob_log_debug( label_fmt( "size: " ) << p.size() );
    ob_log_debug( label_fmt( "vec_size: " ) << v.size() );
}

bool test_pool()
{
    using namespace TestsPool;

    Pool<MyItem> my_pool( 64 );
    std::vector<std::shared_ptr<MyItem>> vec;
    vec.reserve( 128 );
    ob_log_debug( "Pushing" );
    vec.push_back( my_pool.make_shared() );
    dump( my_pool, vec );
    for ( int i = 0; i < 63; ++i )
    {
        vec.push_back( my_pool.make_shared( i ) );
        ob_log_debug( "m_v:" << vec.back()->m_v );
        dump( my_pool, vec );
    }
    ob_log_debug( "Popping" );

    for ( int i = 0; i < 64; ++i )
    {
        vec.pop_back();
        dump( my_pool, vec );
    }

    return true;
}
}
}
