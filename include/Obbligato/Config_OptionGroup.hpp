#pragma once
#ifndef Obbligato_Config_OptionGroup_hpp
#define Obbligato_Config_OptionGroup_hpp

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
#include "Obbligato/Config_Option.hpp"

namespace Obbligato
{
    namespace Config
    {
        class OptionGroup
        {
        private:
            std::string m_prefix;
            std::string m_description;
            
            OptionBase *m_first_option;
            OptionBase *m_last_option;
            
            OptionGroup *m_next_group;
            
        public:
            OptionGroup(
                           std::string prefix,
                           std::string description
                           ) :
            m_prefix( prefix ),
            m_description( description ),
            m_first_option(nullptr),
            m_last_option(nullptr),
            m_next_group(nullptr)
            {
            }
            
            ~OptionGroup()
            {
                delete m_first_option;
            }
            
            void fill_default()
            {
                if( m_first_option )
                {
                    m_first_option->fill_default();
                }
                if( m_next_group )
                {
                    m_next_group->fill_default();
                }
            }
            
            void collect( OptionMap &m )
            {
                if( m_first_option )
                {
                    m_first_option->collect( m );
                }
                if( m_next_group )
                {
                    m_next_group->collect(m);
                }
            }
            
            void help( std::ostream &os ) const
            {
                os << m_description.c_str() << "  (" << m_prefix << "):" << std::endl;
                OptionBase const *o = m_first_option;
                while( o )
                {
                    o->help( os );
                    o = o->next();
                }
                os << std::endl;
            }

            void dump_with_description( std::ostream &os ) const
            {
                dump(os);
            }

            
            void dump( std::ostream &os ) const
            {
                os << "#" << std::endl;
                os << "# options group: " << prefix() << std::endl;
                os << "# " << description() << std::endl;
                os << "#" << std::endl;
                os << std::endl;
                
                OptionBase *o = m_first_option;
                while( o )
                {
                    o->dump( os );
                    o = o->next();
                }
                os << std::endl;
            }
            
            template <
                    typename T
                    >
            OptionGroup & add(
                                 char const * key,
                                 char const * default_value,
                                 char const * description,
                                 T &value
                                 )
            {
                OptionBase *o = new Option<T>(
                            prefix(), key,default_value, description, value
                            );
                if( !m_first_option )
                {
                    m_first_option=o;
                    m_last_option=o;
                }
                else
                {
                    m_last_option->next( o );
                    m_last_option=o;
                }
                return *this;
            }
            
                        
            std::string const &prefix() const
            {
                return m_prefix;
            }
            
            std::string const &description() const
            {
                return m_description;
            }
            
            OptionBase const *first_option() const
            {
                return m_first_option;
            }
            
            OptionBase const *last_option() const
            {
                return m_last_option;
            }
            
            OptionGroup const *next_group() const
            {
                return m_next_group;
            }

            void next_group(OptionGroup *o)
            {
                m_next_group = o;
            }

        };
    }
}

#endif


