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
#include "Obbligato/Logger.hpp"
#include "Obbligato/Config_OptionGroup.hpp"

namespace Obbligato
{
namespace Config
{

class OptionGroups
{
  private:
    bool m_finalized;
    OptionGroup *m_first_group;
    OptionGroup *m_last_group;

    OptionMap m_option_map;

  public:
    OptionGroups() : m_finalized( false ), m_first_group( 0 ), m_last_group( 0 )
    {
    }

    ~OptionGroups()
    {
        delete m_first_group;
    }

    void fill_default()
    {
        if ( !m_finalized && m_first_group )
        {
            m_first_group->fill_default();
        }
    }

    void collect()
    {
        if ( !m_finalized && m_first_group )
        {
            m_first_group->collect( m_option_map );
        }
    }

    void finalize()
    {
        if ( !m_finalized )
        {
            fill_default();
            collect();
        }
        m_finalized = true;
    }

    OptionGroup &add( char const *prefix, char const *description );

    OptionBase &find( std::string const &k );

    void parse( std::string const &k, std::string const &v );

    void parse_file( char const *fname, bool fail_on_invalid_args = true );

    bool parse( char const **argv, std::string const &banner, std::string const &version, std::ostream &os );

    bool parse( char const **argv, char const *version );

    void help( std::ostream &os );

    void help( std::ostream &os, std::string const &prefix );

    void dump_with_description( std::ostream &os ) const;

    void dump_with_description( std::ostream &os, std::string const &prefix );

    void dump( std::ostream &os );

    void dump( std::ostream &os, std::string const &prefix );

    void save_file( const char *fname );

    void save_file( const char *fname, std::string const &prefix );
};
}
}
