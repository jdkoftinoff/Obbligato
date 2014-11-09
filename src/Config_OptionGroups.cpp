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
#include "Obbligato/Config_OptionGroup.hpp"
#include "Obbligato/Config_OptionGroups.hpp"

namespace Obbligato
{
namespace Config
{

OptionGroup &OptionGroups::add( char const *prefix, char const *description )
{
    OptionGroup *o = new OptionGroup( prefix, description );
    if ( !m_first_group )
    {
        m_first_group = o;
        m_last_group = o;
    }
    else
    {
        m_last_group->nextGroup( o );
        m_last_group = o;
    }
    return *o;
}

OptionBase &OptionGroups::find( std::string const &k )
{
    OptionBase *r = 0;
    OptionMap::iterator i = m_option_map.find( k );
    if ( i == m_option_map.end() )
    {
        throw std::invalid_argument( form<2048>( "Unknown option : '%s'", k.c_str() ) );
    }
    else
    {
        r = i->second;
    }
    return *r;
}

void OptionGroups::parse( std::string const &k, std::string const &v ) { find( k ).parse( v ); }

void OptionGroups::parseFile( char const *fname, bool fail_on_invalid_args )
{
    if ( !m_finalized )
    {
        finalize();
    }
    FILE *fp = 0;
#ifdef _WIN32
    fopen_s( &fp, fname, "r" );
#else
    fp = fopen( fname, "r" );
#endif
    if ( fp == NULL )
        return;
    char bline[256];
    size_t line_count = 0;
    while ( fgets( bline, 255, fp ) != NULL )
    {
        bline[255] = 0;
        std::string line( bline );
        ++line_count;
        if ( line.length() > 1 ) // \n is part of the string
        {
            size_t first_non_blank = line.find_first_not_of( "\t " );
            size_t last_non_blank = line.find_last_not_of( "\r\n\t " );

            if ( ( last_non_blank - first_non_blank > 0 ) && line[first_non_blank] != '#' ) // skip blank and comment lines
            {
                size_t end_of_key = line.find_first_of( "\r\n\t =", first_non_blank );
                std::string key = line.substr( first_non_blank, end_of_key - first_non_blank );

                size_t equals_pos = line.find_first_of( "=", end_of_key - 1 );
                size_t first_value_quote_pos = line.find_first_of( '"', equals_pos + 1 );
                std::string value;
                if ( first_value_quote_pos == line.npos )
                {
                    size_t first_non_blank_after_equals = line.find_first_not_of( "\r\n\t ", equals_pos + 1 );
                    size_t last_non_blank = line.find_last_not_of( "\r\n\t " );
                    value = line.substr( first_non_blank_after_equals, last_non_blank - first_non_blank_after_equals );
                }
                else
                {
                    size_t last_quote = line.find_last_of( '"' );
                    value = line.substr( first_value_quote_pos + 1, last_quote - first_value_quote_pos - 1 );
                }

                try
                {
                    parse( key, value );
                }
                catch ( std::invalid_argument &e )
                {
                    if ( fail_on_invalid_args )
                    {
                        fclose( fp );
                        throw std::invalid_argument( form<4096>( "File '%s':%d - %s", fname, line_count, e.what() ) );
                    }
                }
            }
        }
    }
    fclose( fp );
}

bool OptionGroups::parse( char const **argv, std::string const &banner, std::string const &version, std::ostream &os )
{
    if ( !m_finalized )
    {
        finalize();
    }

    try
    {
        while ( *argv )
        {
            if ( ( *argv )[0] == '-' && ( *argv )[1] == '-' )
            {
                char const *pos_equals = strchr( *argv, '=' );
                char const *pos_name = ( *argv ) + 2;
                size_t name_len = ( pos_equals != 0 ? ( pos_equals - pos_name ) : strlen( pos_name ) );
                std::string key( pos_name, name_len );

                if ( key == "version" )
                {
                    os << version << std::endl;
                    return false;
                }

                char const *pos_value = ( pos_equals ? pos_equals + 1 : 0 );
                size_t value_len = ( pos_value ? strlen( pos_value ) : 0 );
                std::string value( pos_value, value_len );

                if ( key == "help" )
                {
                    os << banner << std::endl;
                    os << version << std::endl;
                    if ( value.empty() )
                    {
                        help( os );
                    }
                    else
                    {
                        help( os, value );
                    }
                    return false;
                }

                if ( key == "dump" )
                {
                    if ( value.empty() )
                    {
                        dump( os );
                    }
                    else
                    {
                        dump( os, value );
                    }
                    return false;
                }

                parse( key, value );
            }
            else
            {
                parseFile( *argv );
            }
            ++argv;
        }
    }
    catch ( std::invalid_argument &e )
    {
        throw std::invalid_argument( e.what() + std::string( "( from command line )" ) );
    }

    return true;
}

bool OptionGroups::parse( char const **argv, char const *version )
{
    if ( !m_finalized )
    {
        finalize();
    }

    try
    {
        if ( !parse( argv + 1, std::string( argv[0] ), std::string( version ), std::cout ) )
        {
            return false;
        }
    }
    catch ( std::invalid_argument &e )
    {
        ob_log_error( e.what() );
        return false;
    }
    return true;
}

void OptionGroups::help( std::ostream &os )
{
    OptionGroup const *g = m_first_group;
    while ( g )
    {
        g->help( os );
        g = g->nextGroup();
    }
}

void OptionGroups::help( std::ostream &os, std::string const &prefix )
{
    OptionGroup const *g = m_first_group;
    while ( g )
    {
        if ( g->prefix() == prefix )
        {
            g->help( os );
        }
        g = g->nextGroup();
    }
}

void OptionGroups::dumpWithDescription( std::ostream &os ) const
{
    OptionGroup const *g = m_first_group;
    while ( g )
    {
        g->dumpWithDescription( os );
        g = g->nextGroup();
    }
}

void OptionGroups::dumpWithDescription( std::ostream &os, std::string const &prefix )
{
    OptionGroup const *g = m_first_group;
    while ( g )
    {
        if ( g->prefix() == prefix )
        {
            g->dumpWithDescription( os );
        }
        g = g->nextGroup();
    }
}

void OptionGroups::dump( std::ostream &os )
{
    OptionGroup const *g = m_first_group;
    while ( g )
    {
        g->dump( os );
        g = g->nextGroup();
    }
}

void OptionGroups::dump( std::ostream &os, std::string const &prefix )
{
    OptionGroup const *g = m_first_group;
    while ( g )
    {
        if ( g->prefix() == prefix )
        {
            g->dumpWithDescription( os );
        }
        g = g->nextGroup();
    }
}

void OptionGroups::saveFile( const char *fname )
{
    std::ofstream of( fname );
    dump( of );
}

void OptionGroups::saveFile( const char *fname, std::string const &prefix )
{
    std::ofstream of( fname );
    dump( of, prefix );
}
}
}
