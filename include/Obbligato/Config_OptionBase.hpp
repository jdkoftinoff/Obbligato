#pragma once

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
#include "Obbligato/Form.hpp"
#include "Obbligato/IOStream.hpp"

namespace Obbligato
{
namespace Config
{

class OptionBase;

typedef std::map<std::string, OptionBase *> OptionMap;

class OptionBase
{
    std::string m_prefixed_key;
    std::string m_default_value;
    std::string m_description;

    OptionBase *m_next;

  public:
    OptionBase( std::string prefix_,
                std::string key_,
                std::string default_value_,
                std::string description_ )
        : m_prefixed_key( prefix_ + std::string( "." ) + key_ )
        , m_default_value( default_value_ )
        , m_description( description_ )
        , m_next( nullptr )
    {
    }

    virtual ~OptionBase() { delete m_next; }

    virtual void fillDefault()
    {
        parse( m_default_value );
        if ( m_next )
        {
            m_next->fillDefault();
        }
    }

    void collect( OptionMap &m )
    {
        m[m_prefixed_key] = this;
        if ( m_next )
        {
            m_next->collect( m );
        }
    }

    virtual void help( std::ostream &os ) const
    {
        os << form<1024>( "    %-32s: %s (%s)",
                          m_prefixed_key.c_str(),
                          m_description.c_str(),
                          m_default_value.c_str() ) << std::endl;
    }

    virtual std::string stringValue() const = 0;

    virtual void dumpWithDescription( std::ostream &os ) const = 0;

    virtual void dump( std::ostream &os ) const = 0;

    virtual void parse( std::string const &v ) = 0;

    std::string const &prefixedKey() const { return m_prefixed_key; }

    std::string const &defaultValue() const { return m_default_value; }

    std::string const &description() const { return m_description; }

    OptionBase *next() { return m_next; }

    OptionBase const *next() const { return m_next; }

    void next( OptionBase *n ) { m_next = n; }
};
}
}
