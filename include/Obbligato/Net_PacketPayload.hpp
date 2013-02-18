#pragma once
#ifndef Obbligato_Net_PacketPayload_hpp
#define Obbligato_Net_PacketPayload_hpp

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
#include "Obbligato/IOStream.hpp"
#include "Obbligato/StaticAssert.hpp"

namespace Obbligato
{
    namespace Net
    {
        class PacketPayload
        {
            ssize_t m_payload_data_len;
            uint8_t m_payload_data[1500];
        public:
            typedef uint8_t const * const_pointer;
            typedef uint8_t const & const_reference;
            typedef uint8_t * pointer;
            typedef uint8_t & reference;
            typedef ssize_t size_type;

            class const_iterator : public std::iterator<std::forward_iterator_tag, uint8_t>
            {
                PacketPayload const *m_payload;
                ssize_t m_pos;
            public:
                const_iterator()
                    : m_payload(0),
                      m_pos(0)
                {
                }

                const_iterator( PacketPayload const *payload, ssize_t pos )
                    : m_payload(payload),
                      m_pos(pos)
                {
                }

                const_iterator( const_iterator const & other )
                    : m_payload( other.m_payload ),
                      m_pos( other.m_pos )
                {
                }

                const_iterator const & operator = ( const_iterator const &other )
                {
                    m_payload = other.m_payload;
                    m_pos = other.m_pos;
                    return *this;
                }

                uint8_t operator * () const
                {
                    if( m_pos > (ssize_t)sizeof(m_payload->m_payload_data) || m_pos < 0 )
                    {
                        throw std::invalid_argument("NetPacketPayload::const_iterator pos invalid for *");
                    }

                    return m_payload->m_payload_data[ m_pos ];
                }

                const_iterator &operator ++ ()
                {
                    ++m_pos;
                    return *this;
                }

                const_iterator operator ++ (int)
                {
                    const_iterator prev(*this);
                    ++m_pos;
                    return prev;
                }

                bool operator == ( const_iterator const &other ) const
                {
                    return m_payload == other.m_payload && m_pos == other.m_pos;
                }

                bool operator != ( const_iterator const &other ) const
                {
                    return m_payload != other.m_payload || m_pos != other.m_pos;
                }
            };

            class iterator : public std::iterator<std::forward_iterator_tag, uint8_t>
            {
                PacketPayload *m_payload;
                ssize_t m_pos;
            public:
                iterator()
                    : m_payload(0),
                      m_pos(0)
                {
                }

                iterator( PacketPayload *payload, ssize_t pos )
                    : m_payload(payload),
                      m_pos(pos)
                {
                }

                iterator( iterator const & other )
                    : m_payload( other.m_payload ),
                      m_pos( other.m_pos )
                {
                }

                iterator const & operator = ( iterator const &other )
                {
                    m_payload = other.m_payload;
                    m_pos = other.m_pos;
                    return *this;
                }

                uint8_t &operator * ()
                {
                    if( m_pos > (ssize_t)sizeof(m_payload->m_payload_data) || m_pos < 0 )
                    {
                        throw std::invalid_argument("NetPacketPayload::const_iterator pos invalid for *");
                    }

                    return m_payload->m_payload_data[ m_pos ];
                }

                iterator &operator ++ ()
                {
                    ++m_pos;
                    return *this;
                }

                iterator operator ++ (int)
                {
                    iterator prev(*this);
                    ++m_pos;
                    return prev;
                }

                bool operator == ( iterator const &other ) const
                {
                    return m_payload == other.m_payload && m_pos == other.m_pos;
                }

                bool operator != ( iterator const &other ) const
                {
                    return m_payload != other.m_payload || m_pos != other.m_pos;
                }
            };


            const_iterator begin() const { return const_iterator(this,0); }
            const_iterator end() const { return const_iterator(this,m_payload_data_len); }

            iterator begin() { return iterator(this,0); }
            iterator end() { return iterator(this,m_payload_data_len); }

            const_iterator at(size_type pos) const
            {
                if( pos > m_payload_data_len || pos <0  )
                {
                    throw std::invalid_argument("at pos invalid");
                }
                return const_iterator(this,pos);
            }

            iterator at(size_type pos)
            {
                if( pos > m_payload_data_len || pos <0  )
                {
                    throw std::invalid_argument("at pos invalid");
                }
                return iterator(this,pos);
            }

            PacketPayload()
            {
                clear();
            }

            template <size_t SZ>
            PacketPayload( uint8_t const (&buf)[SZ] )
            {
                OB_STATIC_ASSERT( SZ<=sizeof(m_payload_data) );
                m_payload_data_len = SZ;
                memcpy( m_payload_data, buf, SZ );
            }

            PacketPayload( uint8_t const *buf, size_t sz )
            {
                if( sz > (ssize_t)sizeof(m_payload_data) )
                {
                    throw std::invalid_argument("NetPacketPayload too large");
                }
                m_payload_data_len = sz;
                memcpy( m_payload_data, buf, m_payload_data_len );
            }

            PacketPayload( PacketPayload const &other )
            {
                assign( other );
            }

            void swap( PacketPayload &other )
            {
                if( other.m_payload_data_len > (ssize_t)sizeof(m_payload_data) )
                {
                    throw std::invalid_argument("NetPacketPayload too large");
                }
                std::swap(m_payload_data_len, other.m_payload_data_len );
                for( ssize_t i=0; i<m_payload_data_len; ++i )
                {
                    std::swap(m_payload_data[i],other.m_payload_data[i]);
                }
            }

            void clear()
            {
                m_payload_data_len = 0;
            }

            void assign( PacketPayload const &other )
            {
                if( other.m_payload_data_len > (ssize_t)sizeof(m_payload_data) )
                {
                    throw std::invalid_argument("NetPacketPayload too large");
                }
                memcpy( m_payload_data, other.m_payload_data, other.m_payload_data_len );
            }

            PacketPayload & operator = ( PacketPayload const &other )
            {
                assign(other);
                return *this;
            }

            uint8_t & operator [] ( ssize_t n )
            {
                if( n > m_payload_data_len || n<0)
                {
                    throw std::range_error("NetPacketPayload []");
                }
                return m_payload_data[n];
            }

            uint8_t const & operator [] ( ssize_t n ) const
            {
                if( n > m_payload_data_len || n<0)
                {
                    throw std::range_error("NetPacketPayload []");
                }
                return m_payload_data[n];
            }

            ssize_t size() const
            {
                return m_payload_data_len;
            }

            void size( ssize_t sz )
            {
                if( sz > (ssize_t)sizeof(m_payload_data) )
                {
                    throw std::invalid_argument("NetPacketPayload too large");
                }
                m_payload_data_len = sz;
            }

            template <typename T>
            void extract( T &container, ssize_t pos, ssize_t len ) const
            {
                if( pos<0 || pos>m_payload_data_len )
                {
                    throw std::range_error("NetPacketPayload extact pos");
                }

                ssize_t end_pos=pos+len;
                if( len<0 || (end_pos) < m_payload_data_len )
                {
                    throw std::range_error("NetPacketPayload extact len");
                }

                for( pos=0; pos<end_pos; ++pos )
                {
                    container.push_back( m_payload_data[pos] );
                }
            }

            void push_back( uint8_t v )
            {
                if( m_payload_data_len > (size_type)sizeof(m_payload_data) )
                {
                    throw std::range_error("NetPacketPayload full");
                }
                else
                {
                    m_payload_data[m_payload_data_len++] = v;
                }
            }

            template <template <typename> class ContainerT, typename T >
            void push_back( ContainerT<T> const &container )
            {
                for( typename ContainerT<T>::const_iterator i=container.begin(); i!=container.end(); ++i )
                {
                    push_back( *i );
                }
            }

            uint8_t const * payload_data() const
            {
                return m_payload_data;
            }

            uint8_t * payload_data()
            {
                return m_payload_data;
            }

            ssize_t payload_data_len() const
            {
                return m_payload_data_len;
            }

            void payload_data_len(ssize_t len)
            {
                m_payload_data_len = len;
            }

            ssize_t payload_max_data_len() const
            {
                return sizeof( m_payload_data );
            }

        };

        std::ostream & operator << (std::ostream &o, PacketPayload const &v );

        std::istream & operator >> (std::istream &o, PacketPayload &v );
    }

    namespace IOStream
    {
        template <>
        struct DefaultFormat<Net::PacketPayload>
        {
            typedef OctetBlockFormatter<Net::PacketPayload> formatter_type;
            typedef OctetBlockUnformatter<Net::PacketPayload> unformatter_type;
        };

    }
}

#endif


