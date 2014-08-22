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
#include "Obbligato.hpp"
#include "Obbligato/Test.hpp"

#include "Obbligato/Tests_Config.hpp"
#include "Obbligato/Tests_IOStream.hpp"
#include "Obbligato/Tests_LexicalCast.hpp"
#include "Obbligato/Tests_Logger.hpp"
#include "Obbligato/Tests_Net.hpp"
#include "Obbligato/Tests_Operations.hpp"
#include "Obbligato/Tests_Pool.hpp"
#include "Obbligato/Tests_SIMD.hpp"
#include "Obbligato/Tests_Time.hpp"
#include "Obbligato/Tests_DSP.hpp"

#include "tinyxml2.h"

using namespace tinyxml2;

typedef uint16_t DescriptorType;
typedef uint16_t DescriptorIndex;
typedef uint16_t Doublet;

class XMLWriter
{
  public:
    std::ostream &m_of;

    XMLWriter( std::ostream &of ) : m_of( of )
    {
    }

    bool start( const char *tag, std::string symbol = "" )
    {
        m_of << "<" << tag;
        if ( symbol.length() > 0 )
        {
            m_of << " symbol=\"" << symbol << "\"";
        }
        m_of << ">";
        return true;
    }

    void text( const char *v )
    {
        m_of << v;
    }

    bool end( const char *tag )
    {
        m_of << "</" << tag << ">" << std::endl;
        return true;
    }

    template <typename T>
    friend bool start( XMLWriter &f, T &v, const char *tag, std::string symbol, typename T::avdecc_type * = 0 )
    {
        f.start( tag, symbol );
        return true;
    }

    template <typename T>
    friend bool end( XMLWriter &f, T &v, const char *tag, typename T::avdecc_type * = 0 )
    {
        f.end( tag );
        return true;
    }

    template <typename T>
    friend bool element( XMLWriter &f, std::vector<T> &v, const char *tag )
    {
        for ( auto i : v )
        {
            element( f, i, tag );
        }
        return true;
    }

    inline friend bool element( XMLWriter &f, uint16_t &v, const char *tag )
    {
        f.start( tag );
        f.m_of << std::hex << std::setw( 4 ) << std::setfill( '0' ) << v;
        f.end( tag );
        return true;
    }

    inline friend bool element( XMLWriter &f, uint32_t &v, const char *tag )
    {
        f.start( tag );
        f.m_of << std::hex << std::setw( 8 ) << std::setfill( '0' ) << v;
        f.end( tag );
        return true;
    }
};

#if 0
class XMLReader {
public:

    class ParseError : public std::runtime_error {
    public:
        ParseError(XMLReader const &r, std::string info = "")
            : std::runtime_error(
                std::string("XMLReader::ParseError")
                + Obbligato::to_string(r.m_line+1)  + ":"
                + Obbligato::to_string(r.m_column+1) + ":" + info ) {
        }
    };

    std::istream &m_if;
    int m_line;
    int m_column;
    
    XMLReader( std::istream &i )
        : m_if(i)
        , m_line(0)
        , m_column(0) { std::cerr << "XMLReader()" << std::endl; }
    
    virtual ~XMLReader() {
    }
    
    XMLReader( XMLReader const &other ) = delete;
    
    void consume() {
        while(true) {
            std::istream::char_type c = m_if.peek();
            if( std::isspace(c)) {
                if( c=='\n' ) {
                    m_line++;
                    m_column=0;
                    std::cerr << "line" << std::endl;
                }
                else {
                    m_column++;
                }
                m_if.get();
            }
            else {
                break;
            }
        }
    }
    
    std::string read_token() {
        std::string r;
        consume();
        while(true) {
            std::istream::char_type c = m_if.peek();
            if( std::isalnum(c) || c=='-' || c=='_' || c==':') {
                m_column++;
                r.push_back(m_if.get());
            }
            else {
                break;
            }
        }
        std::cout << "At " << m_line << ":" << m_column << " Got token: " << r << std::endl;
        return r;
    }
    
    std::pair< std::string, std::string > read_attribute() {
        std::pair<std::string,std::string> r;
        consume();
        std::istream::char_type c = m_if.peek();
        if( std::isalnum(c) ) {
            r.first = read_token();
            
            consume();
            c = m_if.peek();
            
            if( c=='=' ) {
                m_column++;
                m_if.get();
                consume();
                std::istream::char_type q;
                q = m_if.peek();
                if( q=='"' || q=='\'' ) {
                    m_column++;
                    m_if.get();
                    r.second = read_token();
                    c = m_if.peek();
                    if( c==q ) {
                        m_column++;
                        m_if.get();
                    }
                    else {
                        throw ParseError(*this,"Error reading attribute");
                    }
                }
            }
        }
        return r;
    }

    void expect(std::string expected_token ) {
        std::string tok = read_token();
        if( tok!=expected_token ) {
            throw ParseError(*this,"unexpected token");
        }
    }
    
    void expect( std::istream::char_type v ) {
        consume();
        std::istream::char_type c = m_if.peek();
        if( c==v ) {
            m_column++;
            m_if.get();
        }
        else {
            throw ParseError(*this,"unexpected character");
        }
    }
    
    bool start( const char *tag, std::string &symbol ) {
        bool r=false;
        std::string token;
        expect('<');
        if( m_if.peek()!='/' ) {
            token = read_token();
            std::pair<std::string,std::string> att = read_attribute();
            expect('>');
            if( token == std::string(tag) ) {
                if( att.first == "symbol" ) {
                    symbol = att.second;
                }
                r=true;
            }
            else {
                throw ParseError(*this,"Could not read tag" );
            }
        }
        else {
            m_if.putback('<');
            r=false;
        }
        return r;
    }
    
    void text( std::string &v ) {
        v.clear();
        while(true) {
            std::istream::char_type c = m_if.peek();
            if( c=='&' ) {
                std::string code = read_token();
                expect(';');
                if( code=="amp") {
                    v.push_back('&');
                } else if( code=="lt" ) {
                    v.push_back('<');
                } else if( code=="gt" ) {
                    v.push_back('>');
                } else if( code=="quot" ) {
                    v.push_back('"');
                } else if( code=="apos" ) {
                    v.push_back('\'');
                } else ParseError(*this,"Bad Ampersand Escape");
            }
            if( c!='<' ) {
                m_column++;
                v.push_back( m_if.get() );
            }
            else {
                break;
            }
        }
    }
    
    bool end( const char *tag ) {
        bool r=true;
        std::string token;
        expect('<');
        expect('/');
        token = read_token();
        expect('>');
        if( token != std::string(tag) ) {
            throw ParseError(*this,"Could not read end tag" );
        }
        return r;
    }

    template <typename T>
    friend bool start( XMLReader &f, T &v, const char *tag, std::string symbol, typename T::avdecc_type * = 0 ) {
        return f.start(tag,symbol);
    }

    template <typename T>
    friend bool end( XMLReader &f, T &v, const char *tag, typename T::avdecc_type * = 0 ) {
        return f.end(tag);
    }

    template <typename T>
    friend int element( XMLReader &f, std::vector<T> &v, const char *tag ) {
        int r=0;
        int n=0;
        std::string symbol;
        do {
            T t;
            n=element(f,t,tag);
            r+=n;
            if(n>0) {
                v.push_back(t);
            }
        } while( n>0 );
        return r;
    }

    inline friend int element( XMLReader &f, uint16_t &v, const char *tag ) {
        int r=0;
        std::string sym;
        r&=f.start(tag,sym);
        if(r) {
            std::string val = f.read_token();
            v = Obbligato::from_string<uint16_t>(val);
            r&=f.end(tag);
        }
        return r;
    }

    inline friend bool element( XMLReader &f, uint32_t &v, const char *tag ) {
        bool r=true;
        std::string sym;
        r&=f.start(tag,sym);
        if(r) {
            std::string val = f.read_token();
            v = Obbligato::from_string<uint32_t>(val);
            r&=f.end(tag);
        }
        return r;
    }

};
#endif

typedef std::map<std::string, std::string> AttributesMap;
typedef std::map<std::string, std::string> ElementMap;

bool element( XMLWriter &f, uint16_t &v, const char *tag, int minOccurs, int maxOccurs )
{
    f.start( tag );
    f.m_of << std::hex << std::setw( 4 ) << std::setfill( '0' ) << v;
    f.end( tag );
    return true;
}

struct Example
{
    typedef Example avdecc_type;
    AttributesMap attributes;

    DescriptorIndex m_stream_index;
    Doublet m_stream_signal;
    Doublet m_cluster_offset;

    template <typename Func>
    friend void each_element( Example &v, Func &f )
    {
        element( "stream_index", v.m_stream_index, f, 1, 1 );
        element( "stream_signal", v.m_stream_signal, f, 1, 1 );
        element( "cluster_offset", v.m_cluster_offset, f, 1, 1 );
    }

    template <typename Context>
    friend void element( std::string tag, Example &v, Context &context, int minOccurs, int maxOccurs )
    {
        while ( start_element( tag, v, v.attributes, context, minOccurs, maxOccurs ) )
        {
            each_element( v, context );
            end_element( tag, v, context );
        }
    }
};

struct SensorMapping
{
    typedef SensorMapping avdecc_type;
    std::string symbol;

    DescriptorIndex stream_index;
    Doublet stream_signal;
    Doublet cluster_offset;

    template <typename Func>
    friend bool each( Func &f, SensorMapping &v )
    {
        bool r = true;
        r &= element( f, v.stream_index, "stream_index" );
        r &= element( f, v.stream_signal, "stream_signal" );
        r &= element( f, v.cluster_offset, "cluster_offset" );
        return r;
    }

    template <typename Func>
    friend bool element( Func &f, SensorMapping &v, const char *tag )
    {
        bool r = true;
        r &= start( f, v, tag, v.symbol );
        if ( r )
        {
            r &= each( f, v );
            r &= end( f, v, tag );
            r = true;
        }
        return r;
    }
};

struct SensorMapDescriptor
{
    typedef SensorMapDescriptor avdecc_type;
    std::string symbol;

    std::vector<SensorMapping> sensor_mapping;

    template <typename Func>
    friend bool each( Func &f, SensorMapDescriptor &v )
    {
        bool r = true;
        r &= element( f, v.sensor_mapping, "sensor_mapping" );
        return r;
    }

    template <typename Func>
    friend bool element( Func &f, SensorMapDescriptor &v, const char *tag )
    {
        bool r = true;
        r &= start( f, v, tag, v.symbol );
        if ( r )
        {
            r &= each( f, v );
            r &= end( f, v, tag );
        }
        return r;
    }
};

struct SensorMapDescriptors
{
    typedef SensorMapDescriptors avdecc_type;
    std::string symbol;

    std::vector<SensorMapDescriptor> sensor_map;

    template <typename Func>
    friend bool each( Func &f, SensorMapDescriptors &v )
    {
        bool r = true;
        r &= element( f, v.sensor_map, "sensor_map" );
        return r;
    }

    template <typename Func>
    friend bool element( Func &f, SensorMapDescriptors &v, const char *tag )
    {
        bool r = true;
        r &= start( f, v, tag, v.symbol );
        if ( r )
        {
            r &= each( f, v );
            r &= end( f, v, tag );
        }
        return r;
    }
};

#if 0

int main(int, char const **argv) {
    try
    {
        SensorMapDescriptors ds;
        
        for( uint16_t j=0; j<1; ++j )
        {
            ds.sensor_map.emplace_back();
            SensorMapDescriptor &d = ds.sensor_map.back();
            d.symbol = std::string("item_") + Obbligato::to_string(j+1);
            
            for( uint16_t i=0; i<10; ++i ) {
                d.sensor_mapping.emplace_back();
                SensorMapping &sm = d.sensor_mapping.back();
                sm.stream_index = i;
                sm.stream_signal = i;
                sm.cluster_offset = i;
            }
        }
        
        
        std::stringstream buf;
        
        XMLWriter w(buf);
        
        element( w, ds, "maps" );
        
        std::cout << buf.str() << std::endl;
        
        XMLDocument doc;
        doc.Parse( buf.str().c_str() );
        doc.SaveFile("output.xml");
        XMLNode *rootNode = doc.FirstChild();
        
        for( XMLNode *i = rootNode->FirstChild(); i!=0; i=i->NextSibling() ) {
            XMLElement *el = i->ToElement();
            std::cout << "node: " << el->Name() << std::endl;
        }
    }
    catch (std::runtime_error &e) {
        std::cout << "Error: " << std::endl;
        std::cout << e.what() << std::endl;
    }
    return 0;

}

#elif 0

int main( int, char const **argv )
{
    try
    {
        SensorMapDescriptors ds;

        for ( uint16_t j = 0; j < 1; ++j )
        {
            ds.sensor_map.emplace_back();
            SensorMapDescriptor &d = ds.sensor_map.back();
            d.symbol = std::string( "item_" ) + Obbligato::to_string( j + 1 );

            for ( uint16_t i = 0; i < 10; ++i )
            {
                d.sensor_mapping.emplace_back();
                SensorMapping &sm = d.sensor_mapping.back();
                sm.stream_index = i;
                sm.stream_signal = i;
                sm.cluster_offset = i;
            }
        }

        std::stringstream buf;

        XMLWriter w( buf );

        element( w, ds, "maps" );

        std::cout << buf.str() << std::endl;

        XMLReader r( buf );

        SensorMapDescriptors ds1;
        element( r, ds1, "maps" );

        std::stringstream buf1;

        XMLWriter w1( buf1 );
        element( w1, ds1, "newmaps" );
        std::cout << buf1.str() << std::endl;
    }
    catch ( XMLReader::ParseError e )
    {
        std::cout << "ParseError: " << std::endl;
        std::cout << e.what() << std::endl;
    }
    return 0;
}
#elif 1

int main( int, char const **argv )
{
    Obbligato::Net::initialize_sockets();
    Obbligato::Test::Harness harness( argv );

    using namespace Obbligato::Tests;
    Obbligato::logger->enable_all();

    ob_log_info( "Starting TestObbligato" );

    OB_RUN_TEST( test_config, "Config" );
    OB_RUN_TEST( test_iostream, "IOStream" );
    OB_RUN_TEST( test_lexicalcast, "LexicalCast" );
    OB_RUN_TEST( test_logger, "Logger" );
    OB_RUN_TEST( test_net, "Net" );
    OB_RUN_TEST( test_operations, "Operations" );
    OB_RUN_TEST( test_pool, "Pool" );
    OB_RUN_TEST( test_simd, "SIMD" );
    OB_RUN_TEST( test_dsp, "DSP" );
    OB_RUN_TEST( test_time, "Time" );

    return harness.result_code();
}

#endif
