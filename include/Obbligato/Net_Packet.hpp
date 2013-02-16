#pragma once
#ifndef Obbligato_Net_Packet_hpp
#define Obbligato_Net_Packet_hpp

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
#include "Obbligato/Net_PacketPayload.hpp"
#include "Obbligato/Net_Address.hpp"
#include "Obbligato/Time.hpp"

namespace Obbligato
{
    namespace Net
    {
        class NetPacket : public NetPacketPayload
        {
            Timestamp m_timestamp;
            NetAddress m_network_port_address;
            NetAddress m_source_address;
            NetAddress m_destination_address;
            uint16_t m_protocol;

        public:
            NetPacket()
                : m_timestamp(0),
                  m_network_port_address(),
                  m_source_address(),
                  m_destination_address(),
                  m_protocol(0)
            {
            }

            NetPacket( NetPacket const &other )
                : NetPacketPayload( other ),
                  m_timestamp( other.m_timestamp ),
                  m_network_port_address( other.m_network_port_address ),
                  m_source_address( other.m_source_address ),
                  m_destination_address( other.m_destination_address ),
                  m_protocol( other.m_protocol )
            {
            }

            void clear()
            {
                NetPacketPayload::clear();
                m_timestamp = 0;
                m_network_port_address = NetAddress();
                m_source_address = NetAddress();
                m_destination_address = NetAddress();
                m_protocol = 0;
            }

            void assign( NetPacket const &other )
            {
                NetPacketPayload::assign( other );
                m_timestamp = other.m_timestamp;
                m_network_port_address = other.m_network_port_address;
                m_source_address = other.m_source_address;
                m_destination_address = other.m_destination_address;
                m_protocol = other.m_protocol;
            }

            void swap( NetPacket &other )
            {
                NetPacketPayload::swap( other );
                std::swap( m_timestamp, other.m_timestamp );
                m_network_port_address.swap( other.m_network_port_address );
                m_source_address.swap( other.m_source_address );
                m_destination_address.swap(other.m_destination_address );
                std::swap( m_protocol, other.m_protocol );
            }

            NetPacket & operator = ( NetPacket const &other )
            {
                assign(other);
                return *this;
            }

            Timestamp timestamp() const
            {
                return m_timestamp;
            }

            void timestamp( Timestamp t )
            {
                m_timestamp = t;
            }

            NetAddress const & network_port_address() const
            {
                return m_network_port_address;
            }

            NetAddress & network_port_address()
            {
                return m_network_port_address;
            }

            void network_port_address( NetAddress const &a )
            {
                m_network_port_address = a;
            }

            NetAddress const & source_address() const
            {
                return m_source_address;
            }

            NetAddress & source_address()
            {
                return m_source_address;
            }

            void source_address( NetAddress const &a )
            {
                m_source_address = a;
            }

            NetAddress const & destination_address() const
            {
                return m_destination_address;
            }

            NetAddress & destination_address()
            {
                return m_destination_address;
            }

            void destination_address( NetAddress const &a )
            {
                m_destination_address = a;
            }

            uint16_t protocol() const
            {
                return m_protocol;
            }

            void protocol( uint16_t p )
            {
                m_protocol = p;
            }

            friend std::ostream &operator << ( std::ostream &o, NetPacket const &v );

            friend std::istream &operator >> ( std::istream &i, NetPacket &v );

        };

    }


}

#endif


