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

namespace Obbligato { namespace Net {

class Packet 
{
    PacketPayload m_payload;
    Timestamp m_timestamp;
    Address m_network_port_address;
    Address m_source_address;
    Address m_destination_address;
    uint16_t m_protocol;

public:
    Packet(ssize_t sz=1500)
        : m_payload(sz),
          m_timestamp(0),
          m_network_port_address(),
          m_source_address(),
          m_destination_address(),
          m_protocol(0)
    {
    }

    Packet( Packet const &other )
        : m_payload( other.m_payload ),
          m_timestamp( other.m_timestamp ),
          m_network_port_address( other.m_network_port_address ),
          m_source_address( other.m_source_address ),
          m_destination_address( other.m_destination_address ),
          m_protocol( other.m_protocol )
    {
    }

    void clear()
    {
        m_payload.clear();
        m_timestamp = 0;
        m_network_port_address = Address();
        m_source_address = Address();
        m_destination_address = Address();
        m_protocol = 0;
    }

    void assign( Packet const &other )
    {
        m_payload = other.m_payload;
        m_timestamp = other.m_timestamp;
        m_network_port_address = other.m_network_port_address;
        m_source_address = other.m_source_address;
        m_destination_address = other.m_destination_address;
        m_protocol = other.m_protocol;
    }

    void swap( Packet &b )
    {
        std::swap( m_payload, b.m_payload );
        std::swap( m_timestamp, b.m_timestamp );
        std::swap( m_network_port_address, b.m_network_port_address );
        std::swap( m_source_address, b.m_source_address );
        std::swap( m_destination_address, b.m_destination_address );
        std::swap( m_protocol, b.m_protocol );
    }

    Packet & operator = ( Packet const &other )
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

    PacketPayload & payload()
    {
        return m_payload;
    }
    
    PacketPayload const & payload() const
    {
        return m_payload;
    }

    Address const & network_port_address() const
    {
        return m_network_port_address;
    }

    Address & network_port_address()
    {
        return m_network_port_address;
    }

    void network_port_address( Address const &a )
    {
        m_network_port_address = a;
    }

    Address const & source_address() const
    {
        return m_source_address;
    }

    Address & source_address()
    {
        return m_source_address;
    }

    void source_address( Address const &a )
    {
        m_source_address = a;
    }

    Address const & destination_address() const
    {
        return m_destination_address;
    }

    Address & destination_address()
    {
        return m_destination_address;
    }

    void destination_address( Address const &a )
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

    friend std::ostream &operator << ( std::ostream &o, Packet const &v );

    friend std::istream &operator >> ( std::istream &i, Packet &v );

};

}}

#endif


