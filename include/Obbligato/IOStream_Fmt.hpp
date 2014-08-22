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
#include "Obbligato/Traits.hpp"
#include "Obbligato/IEEE_Types.hpp"
#include "Obbligato/IOStream_InputState.hpp"
#include "Obbligato/IOStream_OutputState.hpp"
#include "Obbligato/LexicalCast.hpp"

namespace Obbligato
{
namespace IOStream
{

struct Title
{
    std::string m_value;
    Title( std::string v ) : m_value( v )
    {
    }
};

inline Title title( std::string v )
{
    return Title( v );
}

struct Label
{
    std::string m_value;
    Label( std::string v ) : m_value( v )
    {
    }
};

inline Label label( std::string v )
{
    return Label( v );
}

template <typename T>
struct BoolUnformatter
{
    T &m_value;

    BoolUnformatter( T &v ) : m_value( v )
    {
    }
};

template <typename Ch, typename Tr, typename T>
inline std::basic_istream<Ch, Tr> &operator>>( std::basic_istream<Ch, Tr> &i, BoolUnformatter<T> f )
{
    BasicIStreamStateSave<Ch, Tr> stream_state( i );

    bool a;
    i >> std::boolalpha >> a >> std::noboolalpha;
    f.m_value = a;
    return i;
}

template <typename Ch, typename Tr, typename T>
inline std::basic_istream<Ch, Tr> &operator>>( std::basic_istream<Ch, Tr> &i, BoolUnformatter<bool> f )
{
    BasicIStreamStateSave<Ch, Tr> stream_state( i );

    bool a;
    i >> std::boolalpha >> a >> std::noboolalpha;
    f.m_value = a;
    return i;
}

template <typename T>
inline BoolUnformatter<T> bool_unfmt( T &v )
{
    return BoolUnformatter<T>( v );
}

template <typename T>
struct OctetUnformatter
{
    typedef void octetref_void;

    T &m_value;

    OctetUnformatter( T &v ) : m_value( v )
    {
    }
};

template <typename Ch, typename Tr, typename T>
inline std::basic_istream<Ch, Tr> &operator>>( std::basic_istream<Ch, Tr> &i, OctetUnformatter<T> f )
{
    BasicIStreamStateSave<Ch, Tr> stream_state( i );

    uint16_t v;
    i >> std::hex >> v;
    if ( v > 0xff )
    {
        i.clear( std::ios_base::failbit );
    }
    f.m_value = static_cast<IEEE::Octet>( v & 0xff );
    return i;
}

template <typename T>
inline OctetUnformatter<T> octet_unfmt( T v )
{
    return OctetUnformatter<T>( v );
}

template <size_t Digits, typename T>
struct HexUnformatterBase
{
    T &m_value;

    HexUnformatterBase( T &v ) : m_value( v )
    {
    }
};

template <typename T>
struct HexUnformatter : public HexUnformatterBase<sizeof( T ) * 2, T>
{
    HexUnformatter( T &v ) : HexUnformatterBase<sizeof( T ) * 2, T>( v )
    {
    }
};

template <typename T>
HexUnformatter<T> hex_unfmt( T &v )
{
    return HexUnformatter<T>( v );
}

template <typename Ch, typename Tr, typename T, size_t Digits>
inline std::basic_istream<Ch, Tr> &operator>>( std::basic_istream<Ch, Tr> &i, HexUnformatterBase<Digits, T> f )
{
    BasicIStreamStateSave<Ch, Tr> stream_state( i );
    i.unsetf( std::ios_base::basefield );
    i >> f.m_value;
    return i;
}

template <typename Ch, typename Tr, typename T>
inline std::basic_istream<Ch, Tr> &operator>>( std::basic_istream<Ch, Tr> &i, HexUnformatterBase<2, T> f )
{
    BasicIStreamStateSave<Ch, Tr> stream_state( i );
    i.unsetf( std::ios_base::basefield );
    uint16_t v = 0;
    i >> v;
    if ( v > 0xff )
    {
        i.clear( std::ios_base::failbit );
    }
    f.m_value = static_cast<IEEE::Octet>( v & 0xff );
    return i;
}

template <size_t Digits, typename T>
struct BinUnformatterBase
{
    T &m_value;

    BinUnformatterBase( T &v ) : m_value( v )
    {
    }
};

template <typename T>
struct BinUnformatter : public BinUnformatterBase<sizeof( T ) * 8, typename Traits::SizeToStorage<sizeof( T )>::storage_type>
{
    BinUnformatter( T &v ) : BinUnformatterBase<sizeof( T ) * 8, typename Traits::SizeToStorage<sizeof( T )>::storage_type>( v )
    {
    }
};

template <typename T>
BinUnformatter<T> bin_unfmt( T &v )
{
    return HexUnformatter<T>( v );
}

template <typename Ch, typename Tr, size_t N, typename T>
inline std::basic_istream<Ch, Tr> &operator>>( std::basic_istream<Ch, Tr> &i, BinUnformatterBase<N, T> f )
{
    BasicIStreamStateSave<Ch, Tr> stream_state( i );
    T val = 0;
    char c;
    for ( size_t n = 0; n < N; ++n )
    {
        i >> c;
        switch ( c )
        {
        case '0':
            val = val << 1;
            break;
        case '1':
            val = val << 1;
            val |= 1;
            break;
        default:
            i.clear( std::ios_base::failbit );
            break;
        }
    }
    f.m_value = val;

    return i;
}

template <typename T>
struct DecUnformatter
{
    T &m_value;

    DecUnformatter( T &v ) : m_value( v )
    {
    }
};

template <typename T>
inline DecUnformatter<T> dec_unfmt( T &v )
{
    return DecUnformatter<T>( v );
}

template <typename Ch, typename Tr, typename T>
inline std::basic_istream<Ch, Tr> &operator>>( std::basic_istream<Ch, Tr> &i, DecUnformatter<T> f )
{
    i >> f.m_value;
    return i;
}

template <typename T>
struct EUIUnformatter
{
    T &m_value;

    EUIUnformatter( T &v ) : m_value( v )
    {
    }
};

template <typename T>
inline EUIUnformatter<typename T::eui_type> eui_unfmt( T &v )
{
    return EUIUnformatter<T>( v );
}

template <typename Ch, typename Tr, typename T>
inline std::basic_istream<Ch, Tr> &operator>>( std::basic_istream<Ch, Tr> &i, EUIUnformatter<T> f )
{
    std::string s;
    i >> s;
    if ( !f.m_value.from_string( s ) )
    {
        i.clear( std::ios_base::failbit );
    }
    return i;
}

template <typename T>
struct OctetBlockUnformatter
{
    T &m_value;
    OctetBlockUnformatter( T &v ) : m_value( v )
    {
    }
};

template <typename T>
inline OctetBlockUnformatter<T> octetblock_unfmt( T &v )
{
    return OctetBlockUnformatter<T>( v );
}

template <typename Ch, typename Tr, typename T>
inline std::basic_istream<Ch, Tr> &operator>>( std::basic_istream<Ch, Tr> &i, OctetBlockUnformatter<T> f )
{
    BasicIStreamStateSave<Ch, Tr> stream_state( i );
    f.m_value.clear();

    std::string token;
    i >> token;
    if ( token != "{" )
    {
        throw std::invalid_argument( "Expected '{'" );
    }

    while ( i >> token )
    {
        if ( token == "}" )
        {
            break;
        }
        uint8_t val;
        if ( !::Obbligato::IEEE::parse_hex( val, token, 0 ) )
        {
            throw std::invalid_argument( "Expected hex octet" );
        }
        f.m_value.push_back( val );
    }
    return i;
}

template <typename T>
struct StringBlockUnformatter
{
    T &m_value;
    ssize_t m_max_width;
    StringBlockUnformatter( T &v, ssize_t max_width = -1 ) : m_value( v ), m_max_width( max_width )
    {
    }
};

template <typename T>
inline StringBlockUnformatter<T> stringblock_unfmt( T &v, ssize_t max_width = -1 )
{
    return StringBlockUnformatter<T>( v, max_width );
}

template <typename T>
inline std::istream &operator>>( std::istream &i, StringBlockUnformatter<T> f )
{
    IStreamStateSave stream_state( i );

    f.m_value.clear();
    i >> std::noskipws;
    char c;

    enum
    {
        state_plain,
        state_end,
        state_escape,
        state_first_x,
        state_second_x
    } state = state_plain;

    char tmp_x = 0;

    size_t end_pos = 1024 * 1024;
    if ( f.m_max_width > -1 )
    {
        end_pos = f.m_max_width;
    }

    while ( state != state_end && ( i >> c ) )
    {
        if ( f.m_value.length() >= end_pos )
        {
            break;
        }
        switch ( state )
        {
        case state_plain:
        {
            // handle plain text.
            // backslash goes into escape state. double quote ends string.
            switch ( c )
            {
            case '\\':
                state = state_escape;
                break;
            case '"':
                state = state_end;
                break;
            default:
                f.m_value.push_back( c );
                break;
            }
            break;
        }
        case state_escape:
        {
            // handle escape mode.
            // backslash backslash is code for single backslash.
            // backslash n is code for line feed
            // backslash r is code for carriage return
            // backslash t is code for tab
            // backslash " is code for "
            // backslash x is code for two digit hex code, going into first_x
            // state
            // any other code is error
            switch ( c )
            {
            case '\\':
                f.m_value.push_back( '\\' );
                state = state_plain;
                break;
            case '"':
                f.m_value.push_back( '"' );
                state = state_plain;
                break;
            case 'n':
                f.m_value.push_back( '\n' );
                state = state_plain;
                break;
            case 'r':
                f.m_value.push_back( '\r' );
                state = state_plain;
                break;
            case 't':
                f.m_value.push_back( '\t' );
                state = state_plain;
                break;
            case 'x':
                state = state_first_x;
                break;
            default:
                i.clear( std::ios_base::failbit );
                return i;
            }
            break;
        }
        case state_first_x:
        {
            if ( c >= '0' && c <= '9' )
            {
                tmp_x = ( c - '0' ) << 4;
                state = state_second_x;
            }
            else if ( c >= 'a' && c <= 'f' )
            {
                tmp_x = ( c - 'a' + 10 ) << 4;
                state = state_second_x;
            }
            else if ( c >= 'A' && c <= 'F' )
            {
                tmp_x = ( c - 'A' + 10 ) << 4;
                state = state_second_x;
            }
            else
            {
                i.clear( std::ios_base::failbit );
                return i;
            }
            break;
        }
        case state_second_x:
        {
            if ( c >= '0' && c <= '9' )
            {
                tmp_x |= c - '0';
                f.m_value.push_back( tmp_x );
                state = state_plain;
            }
            else if ( c >= 'a' && c <= 'f' )
            {
                tmp_x |= c - 'a' + 10;
                f.m_value.push_back( tmp_x );
                state = state_plain;
            }
            else if ( c >= 'A' && c <= 'F' )
            {
                tmp_x |= c - 'A' + 10;
                f.m_value.push_back( tmp_x );
                state = state_plain;
            }
            else
            {
                i.clear( std::ios_base::failbit );
                return i;
            }
            break;
        }
        case state_end:
        {
            break;
        }
        }
    }
    return i;
}

/////

template <typename T>
struct TitleReaderUnformatter
{
};

template <>
struct TitleReaderUnformatter<std::string>
{
    std::string &m_value;
    size_t m_width;
    TitleReaderUnformatter( string &v, size_t w = 24 ) : m_value( v ), m_width( w )
    {
    }
};

inline TitleReaderUnformatter<std::string> titlereader_unfmt( std::string &v, size_t width = 24 )
{
    return TitleReaderUnformatter<std::string>( v, width );
}

inline TitleReaderUnformatter<std::string> titlereader_unfmt( Title &v, size_t width = 24 )
{
    return TitleReaderUnformatter<std::string>( v.m_value, width );
}

template <typename Ch, typename Tr, typename T>
inline std::basic_istream<Ch, Tr> &operator>>( std::basic_istream<Ch, Tr> &i, TitleReaderUnformatter<T> &f )
{
    BasicIStreamStateSave<Ch, Tr> stream_state( i );
    f.m_value.clear();
    for ( size_t a = 0; a < f.m_width; ++a )
    {
        char c;
        if ( i >> c )
        {
            if ( c == ':' )
            {
                break;
            }
            f.m_value.append( 1, c );
        }
        else
        {
            break;
        }
    }
    return i;
}

template <typename T>
struct TitleUnformatter
{
};

template <>
struct TitleUnformatter<std::string>
{
    std::string const &m_value;
    size_t m_width;
    TitleUnformatter( std::string const &v, size_t w = 24 ) : m_value( v ), m_width( w )
    {
    }
};

inline TitleUnformatter<std::string> title_unfmt( std::string const &v, size_t width = 24 )
{
    return TitleUnformatter<std::string>( v, width );
}

inline TitleUnformatter<std::string> unfmt( Title const &v, size_t width = 24 )
{
    return TitleUnformatter<std::string>( v.m_value, width );
}

template <typename Ch, typename Tr, typename T>
inline std::basic_istream<Ch, Tr> &operator>>( std::basic_istream<Ch, Tr> &i, TitleUnformatter<T> f )
{
    BasicIStreamStateSave<Ch, Tr> stream_state( i );
    std::string v;
    for ( size_t a = 0; a < f.m_width; ++a )
    {
        char c;
        if ( i >> c )
        {
            if ( c == ':' )
            {
                break;
            }
            v.append( 1, c );
        }
        else
        {
            break;
        }
    }
    if ( v != f.m_value )
    {
        throw std::invalid_argument( std::string( "Expected title: " + std::string( f.m_value ) ) );
    }
    return i;
}

template <typename T>
struct LabelReadUnformatter
{
};

template <>
struct LabelReadUnformatter<std::string>
{
    std::string &m_value;
    size_t m_width;
    LabelReadUnformatter( std::string &v, size_t w = 24 ) : m_value( v ), m_width( w )
    {
    }
};

inline LabelReadUnformatter<std::string> labelreader_unfmt( std::string &v, size_t width = 24 )
{
    return LabelReadUnformatter<std::string>( v, width );
}

inline LabelReadUnformatter<std::string> labelreader_unfmt( Label &v, size_t width = 24 )
{
    return LabelReadUnformatter<std::string>( v.m_value, width );
}

template <typename Ch, typename Tr, typename T>
inline std::basic_istream<Ch, Tr> &operator>>( std::basic_istream<Ch, Tr> &i, LabelReadUnformatter<T> f )
{
    BasicIStreamStateSave<Ch, Tr> stream_state( i );
    f.m_value.clear();
    for ( size_t a = 0; a < f.m_width; ++a )
    {
        char c;
        if ( i >> c )
        {
            if ( c == ':' )
            {
                break;
            }
            f.m_value.append( 1, c );
        }
        else
        {
            break;
        }
    }
    return i;
}

template <typename T>
struct LabelUnformatter
{
};

template <>
struct LabelUnformatter<std::string>
{
    std::string const &m_value;
    size_t m_width;
    LabelUnformatter( std::string const &v, size_t w = 24 ) : m_value( v ), m_width( w )
    {
    }
};

template <>
struct LabelUnformatter<Label>
{
    std::string const &m_value;
    size_t m_width;
    LabelUnformatter( Label const &v, size_t w = 24 ) : m_value( v.m_value ), m_width( w )
    {
    }
};

inline LabelUnformatter<std::string> label_unfmt( std::string const &v, size_t width = 24 )
{
    return LabelUnformatter<std::string>( v, width );
}

inline LabelUnformatter<std::string> label_unfmt( Label const &v, size_t width = 24 )
{
    return LabelUnformatter<std::string>( v.m_value, width );
}

template <typename Ch, typename Tr, typename T>
inline std::basic_istream<Ch, Tr> &operator>>( std::basic_istream<Ch, Tr> &i, LabelUnformatter<T> f )
{
    BasicIStreamStateSave<Ch, Tr> stream_state( i );
    std::string v;
    for ( size_t a = 0; a < f.m_width; ++a )
    {
        char c = ':';
        if ( i >> c )
        {
            if ( c == ':' )
            {
                break;
            }
            v.append( 1, c );
        }
        else
        {
            break;
        }
    }
    if ( v != f.m_value )
    {
        throw std::invalid_argument( std::string( "Expected label: " + std::string( f.m_value ) ) );
    }
    return i;
}

template <typename T>
struct BoolFormatter
{
    typedef void boolfmt_void;

    T m_value;

    BoolFormatter( T v ) : m_value( v )
    {
    }
};

template <typename Ch, class Tr, typename T>
inline std::basic_ostream<Ch, Tr> &operator<<( std::basic_ostream<Ch, Tr> &o, BoolFormatter<T> const &f )
{
    BasicOStreamStateSave<Ch, Tr> stream_state( o );

    bool a = ( f.m_value == 0 ) ? false : true;
    o << std::boolalpha << a << std::noboolalpha;
    return o;
}

template <typename Ch, class Tr, typename T>
inline std::basic_ostream<Ch, Tr> &operator<<( std::basic_ostream<Ch, Tr> &o, BoolFormatter<bool> const &f )
{
    BasicOStreamStateSave<Ch, Tr> flags( o );
    o << std::boolalpha << f.m_value << std::noboolalpha;
    return o;
}

template <typename T>
inline BoolFormatter<T> bool_fmt( T v )
{
    return BoolFormatter<T>( v );
}

template <typename T>
struct OctetFormatter
{
    typedef void octetfmt_void;

    T m_value;

    OctetFormatter( T v ) : m_value( v )
    {
    }
};

template <typename Ch, typename Tr, typename T>
inline std::basic_ostream<Ch, Tr> &operator<<( std::basic_ostream<Ch, Tr> &o, OctetFormatter<T> const &f )
{
    BasicOStreamStateSave<Ch, Tr> stream_state( o );

    o << std::setfill( '0' ) << std::setw( 2 ) << std::hex << uint16_t( f.m_value );
    return o;
}

template <typename T>
inline OctetFormatter<T> octet_fmt( T v )
{
    return OctetFormatter<T>( v );
}

template <size_t Digits, typename T>
struct HexFormatterBase
{
    T m_value;

    HexFormatterBase( T v ) : m_value( v )
    {
    }
};

template <typename T>
struct HexFormatter : public HexFormatterBase<sizeof( T ) * 2, T>
{
    HexFormatter( T v ) : HexFormatterBase<sizeof( T ) * 2, T>( v )
    {
    }
};

template <typename T>
HexFormatter<T> hex_fmt( T v )
{
    return HexFormatter<T>( v );
}


template <>
struct HexFormatter<IEEE::EUI48> : public HexFormatterBase<12, uint64_t>
{
    HexFormatter( IEEE::EUI48 v ) : HexFormatterBase<12, uint64_t>( v.to_octlet() )
    {
    }
};

template <>
struct HexFormatter<IEEE::EUI64> : public HexFormatterBase<16, uint64_t>
{
    HexFormatter( IEEE::EUI64 v ) : HexFormatterBase<16, uint64_t>( v.to_octlet() )
    {
    }
};


template <typename Ch, typename Tr, size_t Digits, typename T>
inline std::basic_ostream<Ch, Tr> &operator<<( std::basic_ostream<Ch, Tr> &o, HexFormatterBase<Digits, T> const &f )
{
    BasicOStreamStateSave<Ch, Tr> stream_state( o );

    if ( f.m_value )
    {
        o << std::internal << std::setw( Digits + 2 ) << std::showbase << std::setfill( '0' ) << std::hex << f.m_value;
    }
    else // workaround for standard library insistence to not print 0x if
         // value is 0
    {
        o << "0x" << std::internal << std::setw( Digits ) << std::setfill( '0' ) << std::hex << f.m_value;
    }
    return o;
}


template <typename Ch, typename Tr>
inline std::basic_ostream<Ch, Tr> &operator<<( std::basic_ostream<Ch, Tr> &o, IEEE::EUI64 const &v )
{
    BasicOStreamStateSave<Ch, Tr> stream_state( o );

    for ( int i = 0; i < v.size; ++i )
    {
        o << hex_fmt( v.m_value[i] );
        if ( i != 7 )
        {
            o << ':';
        }
    }
    return o;
}

template <typename Ch, typename Tr>
inline std::basic_ostream<Ch, Tr> &operator<<( std::basic_ostream<Ch, Tr> &o, IEEE::EUI48 const &v )
{
    BasicOStreamStateSave<Ch, Tr> stream_state( o );

    for ( int i = 0; i < v.size; ++i )
    {
        o << std::internal << std::setw( 2 ) << std::setfill( '0' ) << std::hex << (uint16_t)v.m_value[i];

        if ( i != 7 )
        {
            o << ':';
        }
    }
    return o;
}

template <typename Ch, typename Tr, typename T>
inline std::basic_ostream<Ch, Tr> &operator<<( std::basic_ostream<Ch, Tr> &o, HexFormatterBase<2, T> const &f )
{
    BasicOStreamStateSave<Ch, Tr> stream_state( o );

    if ( f.m_value )
    {
        o << std::internal << std::setw( 2 + 2 ) << std::showbase << std::setfill( '0' ) << std::hex << doublet( f.m_value );
    }
    else // workaround for standard library insistence to not print 0x if
         // value is 0
    {
        o << "0x" << std::internal << std::setw( 2 ) << std::setfill( '0' ) << std::hex << doublet( f.m_value );
    }
    return o;
}

template <size_t Digits, typename T>
struct BinFormatterBase
{
    T m_value;

    BinFormatterBase( T v ) : m_value( v )
    {
    }
};

template <typename T>
struct BinFormatter : public BinFormatterBase<sizeof( T ) * 8, typename Traits::SizeToStorage<sizeof( T )>::storage_type>
{
    BinFormatter( T v ) : BinFormatterBase<sizeof( T ) * 8, typename Traits::SizeToStorage<sizeof( T )>::storage_type>( v )
    {
    }
};

template <typename T>
BinFormatter<T> bin_fmt( T v )
{
    return BinFormatter<T>( v );
}

template <typename Ch, typename Tr, size_t Digits, typename T>
inline std::basic_ostream<Ch, Tr> &operator<<( std::basic_ostream<Ch, Tr> &o, BinFormatterBase<Digits, T> const &f )
{
    BasicOStreamStateSave<Ch, Tr> stream_state( o );
    T bit = T( 1 ) << ( Digits - 1 );
    for ( size_t i = 0; i < Digits; ++i )
    {
        if ( ( f.m_value & bit ) != 0 )
        {
            o << '1';
        }
        else
        {
            o << '0';
        }
        bit = bit >> 1;
    }
    return o;
}

template <typename T>
struct DecFormatter
{
    T m_value;

    DecFormatter( T v ) : m_value( v )
    {
    }
};

template <typename T>
inline DecFormatter<T> dec_fmt( T v )
{
    return DecFormatter<T>( v );
}

template <typename Ch, typename Tr, typename T>
inline std::basic_ostream<Ch, Tr> &operator<<( std::basic_ostream<Ch, Tr> &o, DecFormatter<T> const &f )
{
    BasicOStreamStateSave<Ch, Tr> stream_state( o );
    o << std::right << std::setw( 16 ) << std::setfill( ' ' ) << std::dec << f.m_value;
    return o;
}

template <typename T>
struct EUIFormatter
{
    T m_value;
    char m_separator;

    EUIFormatter( T v, char separator = '-' ) : m_value( v ), m_separator( separator )
    {
    }
};

template <typename T>
inline EUIFormatter<typename T::eui_type> eui_fmt( T v, char separator = '-' )
{
    return EUIFormatter<T>( v, separator );
}

template <typename Ch, typename Tr, typename T>
inline std::basic_ostream<Ch, Tr> &operator<<( std::basic_ostream<Ch, Tr> &o, EUIFormatter<T> const &f )
{
    BasicOStreamStateSave<Ch, Tr> stream_state( o );

    o << f.m_value.to_string( f.m_separator );
    return o;
}

template <typename T>
struct OctetBlockFormatter
{
    T const &m_value;
    OctetBlockFormatter( T const &v ) : m_value( v )
    {
    }
};

template <typename T>
inline OctetBlockFormatter<T> octetblock_fmt( T const &v )
{
    return OctetBlockFormatter<T>( v );
}

template <typename Ch, typename Tr, typename T>
inline std::basic_ostream<Ch, Tr> &operator<<( std::basic_ostream<Ch, Tr> &o, OctetBlockFormatter<T> const &f )
{
    BasicOStreamStateSave<Ch, Tr> stream_state( o );

    typedef typename T::const_iterator cit;
    o << "{ ";
    for ( cit i = f.m_value.begin(); i != f.m_value.end(); ++i )
    {
        o << octet_fmt( *i ) << " ";
    }
    o << "}";
    return o;
}

/////

template <typename T>
struct StringBlockFormatter
{
    T const &m_value;
    ssize_t m_max_width;
    StringBlockFormatter( T const &v, ssize_t max_width = -1 ) : m_value( v ), m_max_width( max_width )
    {
    }
};

template <typename T>
inline StringBlockFormatter<T> stringblock_fmt( T const &v, ssize_t max_width = -1 )
{
    return StringBlockFormatter<T>( v, max_width );
}

template <typename T>
inline std::ostream &operator<<( std::ostream &o, StringBlockFormatter<T> const &f )
{
    OStreamStateSave stream_state( o );

    std::string raw = to_string( f.m_value );
    std::string printable;
    ssize_t length_to_use = raw.length();
    if ( f.m_max_width > 0 && f.m_max_width < length_to_use )
    {
        length_to_use = f.m_max_width;
    }
    for ( ssize_t i = 0; i < length_to_use; ++i )
    {
        char c = raw[i];
        if ( c == 0 )
        {
            break;
        }
        if ( c == '\\' )
        {
            printable.push_back( '\\' );
        }
        else if ( c == '"' )
        {
            printable.append( "\\\"" );
        }
        else if ( c == '\r' )
        {
            printable.append( "\\r" );
        }
        else if ( c == '\n' )
        {
            printable.append( "\\n" );
        }
        else if ( c == '\t' )
        {
            printable.append( "\\t" );
        }
        else if ( c < ' ' && c >= 0 )
        {
            printable.append( "\\x" );
            IEEE::put_hex( printable, uint8_t( c ) );
        }
        else
        {
            printable.push_back( c );
        }
    }
    o << printable;
    return o;
}

template <typename T>
struct TitleFormatter
{
};

template <>
struct TitleFormatter<char *>
{
    const char *text;
    size_t width;

    TitleFormatter( char const *text_, size_t width_ = 12 ) : text( text_ ), width( width_ )
    {
    }
};

template <>
struct TitleFormatter<std::string>
{
    std::string const &text;
    size_t width;

    TitleFormatter( std::string const &text_, size_t width_ = 12 ) : text( text_ ), width( width_ )
    {
    }
};

template <>
struct TitleFormatter<Title>
{
    std::string const &text;
    size_t width;

    TitleFormatter( Title const &text_, size_t width_ = 12 ) : text( text_.m_value ), width( width_ )
    {
    }
};

inline TitleFormatter<char *> title_fmt( char const *text, size_t width = 12 )
{
    return TitleFormatter<char *>( text, width );
}

inline TitleFormatter<std::string> title_fmt( std::string text, size_t width = 12 )
{
    return TitleFormatter<std::string>( text, width );
}

inline TitleFormatter<Title> fmt( Title const &text, size_t width = 0 )
{
    return TitleFormatter<Title>( text, width );
}

template <typename Ch, typename Tr, typename T>
inline std::basic_ostream<Ch, Tr> &operator<<( std::basic_ostream<Ch, Tr> &o, TitleFormatter<T> const &f )
{
    BasicOStreamStateSave<Ch, Tr> stream_state( o );
    o << std::left << std::setfill( ' ' ) << std::setw( int( f.width ) ) << f.text << ": ";
    return o;
}

template <typename T>
struct LabelFormatter
{
};

template <>
struct LabelFormatter<char *>
{
    char const *text;
    size_t width;

    LabelFormatter( char const *text_, size_t width_ = 24 ) : text( text_ ), width( width_ )
    {
    }
};

template <>
struct LabelFormatter<string>
{
    string const &text;
    size_t width;

    LabelFormatter( string const &text_, size_t width_ = 24 ) : text( text_ ), width( width_ )
    {
    }
};

template <>
struct LabelFormatter<Label>
{
    string const &text;
    size_t width;

    LabelFormatter( Label const &text_, size_t width_ = 24 ) : text( text_.m_value ), width( width_ )
    {
    }
};

inline LabelFormatter<char *> label_fmt( char const *text, size_t width = 24 )
{
    return LabelFormatter<char *>( text, width );
}

inline LabelFormatter<std::string> label_fmt( std::string text, size_t width = 24 )
{
    return LabelFormatter<string>( text, width );
}

inline LabelFormatter<Label> label_fmt( Label const &text, size_t width = 24 )
{
    return LabelFormatter<Label>( text, width );
}

template <typename Ch, typename Tr, typename T>
inline std::basic_ostream<Ch, Tr> &operator<<( std::basic_ostream<Ch, Tr> &o, LabelFormatter<T> const &f )
{
    BasicOStreamStateSave<Ch, Tr> stream_state( o );
    o << std::right << std::setfill( ' ' ) << std::setw( int( f.width ) ) << f.text << ": ";
    return o;
}
}
}
