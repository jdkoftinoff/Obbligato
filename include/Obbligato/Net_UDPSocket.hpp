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
#include "Obbligato/Pool.hpp"
#include "Obbligato/Net_Address.hpp"
#include "Obbligato/Net_PacketSocket.hpp"
#include "Obbligato/Net_Packet.hpp"

namespace Obbligato {
namespace Net {

class UDPSocket : public PacketSocket {
  private:
    socket_fd_t m_fd;

    Address m_local_addr;
    Address m_default_dest_addr;

    shared_ptr< Pool<Packet> > m_pool;
    
  public:

    UDPSocket(
        shared_ptr< Pool<Packet> > &pool,
        Address local_addr,
        Address default_dest_addr
        );

    UDPSocket(UDPSocket &&other)
        :
          m_fd(std::move(other.m_fd))
        , m_local_addr(std::move(other.m_local_addr))
        , m_default_dest_addr(std::move(other.m_default_dest_addr))
        , m_pool( std::move(other.m_pool) ) {}

    UDPSocket const &operator=(UDPSocket &&other) {
        m_fd = std::move(other.m_fd);
        m_local_addr = std::move(m_local_addr);
        m_default_dest_addr = std::move(m_default_dest_addr);
        m_pool = std::move(other.m_pool );
        return *this;
    }

    /// Close and destroy the socket
    virtual ~UDPSocket();

    /// Returns true if the object is ready for business
    virtual bool is_open() const { return m_fd != INVALID_SOCKET; }

    /// Close the socket
    virtual void close();

    /// Time passed
    virtual void tick(Timestamp);

    /// Get the local socket address
    virtual Address const &local_address() const;

    /// Get the default destination address
    virtual Address const &destination_address() const;

    /// Send the packet referenced by pkt.
    virtual void send(PacketPtr const &pkt);

    /// Attempt to receive a packet from the network
    virtual PacketPtr recv();

    /// Join the specified multicast address
    virtual bool join_multicast(const char *interface_name,
                                Address const &address);

    /// get the current file descriptor of the socket
    virtual socket_fd_t fd() const { return m_fd; }
};

inline UDPSocket make_udpsocket(
    shared_ptr<Pool<Packet>> &pool,Address local_address,
    Address default_dest_addr) {
    return UDPSocket(pool,local_address, default_dest_addr);
}


}
}
