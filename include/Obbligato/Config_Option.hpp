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
#include "Obbligato/Config_OptionBase.hpp"
#include "Obbligato/LexicalCast.hpp"

namespace Obbligato
{
namespace Config
{

template <typename T>
class Option : public OptionBase
{
  private:
    T &m_value;

  public:
    Option( std::string group,
            std::string key,
            std::string default_value,
            std::string description,
            T &value )
        : OptionBase( group, key, default_value, description )
        , m_value( value )
    {
    }

    template <typename U>
    Option<U> &add( char const *prefix,
                    char const *key,
                    char const *default_value,
                    char const *description,
                    U &value )
    {
        next( new Option<U>(
            prefix, key, default_value, description, value ) );
        return *next();
    }

    template <typename U>
    Option<U> &add( char const *prefix,
                    char const *key,
                    char const *default_value,
                    char const *description,
                    U *value )
    {
        Option<U> *n = new Option<U>(
            prefix, key, default_value, description, value );
        next( n );
        return next();
    }

    virtual std::string stringValue() const
    {
        std::stringstream oss;
        oss << IOStream::stringblock_fmt( m_value );
        return oss.str();
    }

    T const &value() const { return m_value; }

    T &value() { return m_value; }

    template <typename U>
    U staticCastValue() const
    {
        return static_cast<U>( m_value );
    }

    virtual void dumpWithDescription( std::ostream &os ) const
    {
        os << "# " << description() << std::endl;
        os << prefixedKey() << "=\"" << stringValue() << "\""
           << std::endl;
        os << std::endl;
    }

    virtual void dump( std::ostream &os ) const
    {
        os << prefixedKey() << "=\"" << stringValue() << "\""
           << std::endl;
    }

    virtual void parse( std::string const &v )
    {
        std::stringstream iss;
        iss.str( v );
        try
        {
            iss >> value();
        }
        catch ( std::invalid_argument & )
        {
            throw std::invalid_argument(
                form<4096>( "'%s' is an invalid value for option '%s'",
                            v.c_str(),
                            prefixedKey().c_str() ) );
        }
    }
};
}
}
