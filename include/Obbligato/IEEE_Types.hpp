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

namespace Obbligato
{
namespace IEEE
{

typedef uint8_t Octet;
typedef uint16_t Doublet;
typedef uint32_t Quadlet;
typedef uint64_t Octlet;

typedef Doublet EtherType;

bool parse_hexdigit( uint8_t &val, char c );

bool parse_hex( Octet &val, char c1, char c2 );

bool parse_hex( Octet &val,
                std::string const &s,
                std::string::size_type pos = 0 );

void put_hexdigit( std::string &result, Octet v );

void put_hex( std::string &result, Octet v );

bool parse_hex( Doublet &val,
                std::string const &s,
                std::string::size_type pos = 0 );

void put_hex( std::string &result, Doublet v );

bool parse_hex( Quadlet &val,
                std::string const &s,
                std::string::size_type pos = 0 );

void put_hex( std::string &result, Quadlet v );

bool parse_hex( Octlet &val,
                std::string const &s,
                std::string::size_type pos = 0 );

void put_hex( std::string &result, Octlet v );

class EUI48;
class MAC48;
class EUI64;

class EUI48
{
  public:
    typedef EUI48 eui_type;
    static const size_t size = 6;

    EUI48( Octet const v[6] )
    {
        m_value[0] = v[0];
        m_value[1] = v[1];
        m_value[2] = v[2];
        m_value[3] = v[3];
        m_value[4] = v[4];
        m_value[5] = v[5];
    }

    EUI48( Octlet v = 0xffffffffffffULL )
    {
        m_value[0] = ( ( v >> 40 ) & 0xff );
        m_value[1] = ( ( v >> 32 ) & 0xff );
        m_value[2] = ( ( v >> 24 ) & 0xff );
        m_value[3] = ( ( v >> 16 ) & 0xff );
        m_value[4] = ( ( v >> 8 ) & 0xff );
        m_value[5] = ( ( v >> 0 ) & 0xff );
    }

    EUI48( MAC48 const &o );
    EUI48( EUI64 const &o );

    void store( Octet v[6] ) const
    {
        v[0] = m_value[0];
        v[1] = m_value[1];
        v[2] = m_value[2];
        v[3] = m_value[3];
        v[4] = m_value[4];
        v[5] = m_value[5];
    }

    EUI48 const &operator=( EUI48 const &other )
    {
        assign( other );
        return *this;
    }

    void assign( EUI48 const &other )
    {
        m_value[0] = other.m_value[0];
        m_value[1] = other.m_value[1];
        m_value[2] = other.m_value[2];
        m_value[3] = other.m_value[3];
        m_value[4] = other.m_value[4];
        m_value[5] = other.m_value[5];
    }

    void swap( EUI48 o )
    {
        using namespace std;
        for ( size_t i = 0; i < sizeof( m_value ); ++i )
        {
            std::swap( m_value[i], o.m_value[i] );
        }
    }

    Octlet toOctlet() const
    {
        Octlet r = 0;
        r |= static_cast<Octlet>( m_value[0] ) << 40;
        r |= static_cast<Octlet>( m_value[1] ) << 32;
        r |= static_cast<Octlet>( m_value[2] ) << 24;
        r |= static_cast<Octlet>( m_value[3] ) << 16;
        r |= static_cast<Octlet>( m_value[4] ) << 8;
        r |= static_cast<Octlet>( m_value[5] ) << 0;
        return r;
    }

    std::string toString( char separator = '-' ) const;

    bool fromString( std::string const &v );

    Octet m_value[6];
};

class EUI64
{
  public:
    typedef EUI64 eui_type;
    static const size_t size = 8;

    EUI64( Octet const v[8] )
    {
        m_value[0] = v[0];
        m_value[1] = v[1];
        m_value[2] = v[2];
        m_value[3] = v[3];
        m_value[4] = v[4];
        m_value[5] = v[5];
        m_value[6] = v[6];
        m_value[7] = v[7];
    }

    EUI64( Octlet v = 0xffffffffffffffffULL )
    {
        m_value[0] = ( ( v >> 56 ) & 0xff );
        m_value[1] = ( ( v >> 48 ) & 0xff );
        m_value[2] = ( ( v >> 40 ) & 0xff );
        m_value[3] = ( ( v >> 32 ) & 0xff );
        m_value[4] = ( ( v >> 24 ) & 0xff );
        m_value[5] = ( ( v >> 16 ) & 0xff );
        m_value[6] = ( ( v >> 8 ) & 0xff );
        m_value[7] = ( ( v >> 0 ) & 0xff );
    }

    EUI64( EUI48 const &o );
    EUI64( MAC48 const &o );

    EUI64 const &operator=( EUI64 const &other )
    {
        assign( other );
        return *this;
    }

    void assign( EUI64 const &other )
    {
        m_value[0] = other.m_value[0];
        m_value[1] = other.m_value[1];
        m_value[2] = other.m_value[2];
        m_value[3] = other.m_value[3];
        m_value[4] = other.m_value[4];
        m_value[5] = other.m_value[5];
        m_value[6] = other.m_value[6];
        m_value[7] = other.m_value[7];
    }

    void store( Octet v[8] ) const
    {
        v[0] = m_value[0];
        v[1] = m_value[1];
        v[2] = m_value[2];
        v[3] = m_value[3];
        v[4] = m_value[4];
        v[5] = m_value[5];
        v[6] = m_value[6];
        v[7] = m_value[7];
    }

    void swap( EUI64 o )
    {
        using namespace std;
        for ( size_t i = 0; i < sizeof( m_value ); ++i )
        {
            std::swap( m_value[i], o.m_value[i] );
        }
    }

    Octlet toOctlet() const
    {
        Octlet r = 0;
        r |= static_cast<Octlet>( m_value[0] ) << 56;
        r |= static_cast<Octlet>( m_value[1] ) << 48;
        r |= static_cast<Octlet>( m_value[2] ) << 40;
        r |= static_cast<Octlet>( m_value[3] ) << 32;
        r |= static_cast<Octlet>( m_value[4] ) << 24;
        r |= static_cast<Octlet>( m_value[5] ) << 16;
        r |= static_cast<Octlet>( m_value[6] ) << 8;
        r |= static_cast<Octlet>( m_value[7] ) << 0;
        return r;
    }

    std::string toString( char separator = '-' ) const;

    bool fromString( std::string const &v );

    Octet m_value[8];
};

class MAC48
{
  public:
    typedef MAC48 eui_type;
    static const size_t size = 6;

    MAC48( Octet const v[6] )
    {
        m_value[0] = v[0];
        m_value[1] = v[1];
        m_value[2] = v[2];
        m_value[3] = v[3];
        m_value[4] = v[4];
        m_value[5] = v[5];
    }

    MAC48( Octlet v = 0xffffffffffffULL )
    {
        m_value[0] = ( ( v >> 40 ) & 0xff );
        m_value[1] = ( ( v >> 32 ) & 0xff );
        m_value[2] = ( ( v >> 24 ) & 0xff );
        m_value[3] = ( ( v >> 16 ) & 0xff );
        m_value[4] = ( ( v >> 8 ) & 0xff );
        m_value[5] = ( ( v >> 0 ) & 0xff );
    }

    MAC48( EUI64 const &o );

    MAC48 const &operator=( MAC48 const &other )
    {
        assign( other );
        return *this;
    }

    void assign( MAC48 const &other )
    {
        m_value[0] = other.m_value[0];
        m_value[1] = other.m_value[1];
        m_value[2] = other.m_value[2];
        m_value[3] = other.m_value[3];
        m_value[4] = other.m_value[4];
        m_value[5] = other.m_value[5];
    }

    void store( Octet v[6] ) const
    {
        v[0] = m_value[0];
        v[1] = m_value[1];
        v[2] = m_value[2];
        v[3] = m_value[3];
        v[4] = m_value[4];
        v[5] = m_value[5];
    }

    Octlet toOctlet() const
    {
        Octlet r = 0;
        r |= static_cast<Octlet>( m_value[0] ) << 40;
        r |= static_cast<Octlet>( m_value[1] ) << 32;
        r |= static_cast<Octlet>( m_value[2] ) << 24;
        r |= static_cast<Octlet>( m_value[3] ) << 16;
        r |= static_cast<Octlet>( m_value[4] ) << 8;
        r |= static_cast<Octlet>( m_value[5] ) << 0;
        return r;
    }

    void swap( MAC48 o )
    {
        using namespace std;
        for ( size_t i = 0; i < sizeof( m_value ); ++i )
        {
            std::swap( m_value[i], o.m_value[i] );
        }
    }

    std::string toString( char separator = '-' ) const;

    bool fromString( std::string const &v );

    Octet m_value[6];
};

inline EUI64::EUI64( EUI48 const &o )
{
    m_value[0] = o.m_value[0];
    m_value[1] = o.m_value[1];
    m_value[2] = o.m_value[2];
    m_value[3] = 0xff;
    m_value[4] = 0xfe;
    m_value[5] = o.m_value[3];
    m_value[6] = o.m_value[4];
    m_value[7] = o.m_value[5];
}

inline EUI64::EUI64( MAC48 const &o )
{
    m_value[0] = o.m_value[0];
    m_value[1] = o.m_value[1];
    m_value[2] = o.m_value[2];
    m_value[3] = 0xff;
    m_value[4] = 0xff;
    m_value[5] = o.m_value[3];
    m_value[6] = o.m_value[4];
    m_value[7] = o.m_value[5];
}

inline EUI48::EUI48( EUI64 const &o )
{
    if ( o.m_value[3] != 0xff || o.m_value[4] != 0xfe )
    {
        throw std::invalid_argument(
            "EUI64 is not convertible to EUI48" );
    }
    m_value[0] = o.m_value[0];
    m_value[1] = o.m_value[1];
    m_value[2] = o.m_value[2];
    m_value[3] = o.m_value[3];
    m_value[4] = o.m_value[4];
    m_value[5] = o.m_value[5];
}

inline MAC48::MAC48( EUI64 const &o )
{
    if ( o.m_value[3] != 0xff || o.m_value[4] != 0xff )
    {
        throw std::invalid_argument(
            "EUI64 is not convertible to MAC48" );
    }
    m_value[0] = o.m_value[0];
    m_value[1] = o.m_value[1];
    m_value[2] = o.m_value[2];
    m_value[3] = o.m_value[3];
    m_value[4] = o.m_value[4];
    m_value[5] = o.m_value[5];
}
}
}
