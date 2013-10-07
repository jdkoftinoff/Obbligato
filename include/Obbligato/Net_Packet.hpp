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
#include "Obbligato/Net_PacketPayload.hpp"
#include "Obbligato/Net_Address.hpp"
#include "Obbligato/Time.hpp"

namespace Obbligato {
namespace Net {

class Packet {
    PacketPayload m_payload;
    Timestamp m_timestamp;
    Address m_network_port_address;
    Address m_source_address;
    Address m_destination_address;
    uint16_t m_protocol;

  public:
    Packet()
        : m_payload(), m_timestamp(0), m_network_port_address(),
          m_source_address(), m_destination_address(), m_protocol(0) {}

    Packet(Packet &&other)
        : m_payload(std::move(other.m_payload)),
          m_timestamp(std::move(other.m_timestamp)),
          m_network_port_address(std::move(other.m_network_port_address)),
          m_source_address(std::move(other.m_source_address)),
          m_destination_address(std::move(other.m_destination_address)),
          m_protocol(std::move(other.m_protocol)) {}

    Packet(Packet const &other)
        : m_payload(other.m_payload), m_timestamp(other.m_timestamp),
          m_network_port_address(other.m_network_port_address),
          m_source_address(other.m_source_address),
          m_destination_address(other.m_destination_address),
          m_protocol(other.m_protocol) {}

    void clear() {
        m_payload.data.clear();
        m_timestamp = 0;
        m_network_port_address = Address();
        m_source_address = Address();
        m_destination_address = Address();
        m_protocol = 0;
    }

    void assign(Packet const &other) {
        m_payload = other.m_payload;
        m_timestamp = other.m_timestamp;
        m_network_port_address = other.m_network_port_address;
        m_source_address = other.m_source_address;
        m_destination_address = other.m_destination_address;
        m_protocol = other.m_protocol;
    }

    Packet const &operator=(Packet const &other) {
        assign(other);
        return *this;
    }

    Packet const &operator=(Packet &&other) {
        m_payload = std::move(other.m_payload);
        m_timestamp = std::move(other.m_timestamp);
        m_network_port_address = std::move(other.m_network_port_address);
        m_source_address = std::move(other.m_source_address);
        m_destination_address = std::move(other.m_destination_address);
        m_protocol = std::move(other.m_protocol);
        return *this;
    }

    void swap(Packet &b) {
        using std::swap;
        swap(m_payload, b.m_payload);
        swap(m_timestamp, b.m_timestamp);
        swap(m_network_port_address, b.m_network_port_address);
        swap(m_source_address, b.m_source_address);
        swap(m_destination_address, b.m_destination_address);
        swap(m_protocol, b.m_protocol);
    }

    Timestamp timestamp() const { return m_timestamp; }

    void set_timestamp(Timestamp t) { m_timestamp = t; }

    PacketPayload &payload() { return m_payload; }

    PacketPayload const &payload() const { return m_payload; }

    PacketPayload const &set_payload() const { return m_payload; }

    Address const &network_port_address() const {
        return m_network_port_address;
    }

    Address &network_port_address() { return m_network_port_address; }

    void set_network_port_address(Address const &a) {
        m_network_port_address = a;
    }

    Address const &source_address() const { return m_source_address; }

    Address &source_address() { return m_source_address; }

    void set_source_address(Address const &a) { m_source_address = a; }

    Address const &destination_address() const { return m_destination_address; }

    Address &destination_address() { return m_destination_address; }

    void set_destination_address(Address const &a) {
        m_destination_address = a;
    }

    uint16_t protocol() const { return m_protocol; }

    void set_protocol(uint16_t p) { m_protocol = p; }

    friend std::ostream &operator<<(std::ostream &o, Packet const &v);

    friend std::istream &operator>>(std::istream &i, Packet &v);
};

std::ostream &operator<<(std::ostream &o, Packet const &v);

std::istream &operator>>(std::istream &i, Packet &v);

typedef std::shared_ptr<Packet> PacketPtr;
}
}
