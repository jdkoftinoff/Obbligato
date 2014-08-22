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
#include "Obbligato/Net_Socket.hpp"
#include "Obbligato/Net_PacketSocket.hpp"

namespace Obbligato
{
namespace Net
{

class RawSocketLinux : public PacketSocket
{
  private:
    socket_fd_t m_fd;

    Address m_local_addr;
    Address m_default_dest_addr;

  public:
    RawSocketLinux( Address local_addr, Address default_dest_addr );

    /// Close and destroy the socket
    virtual ~RawSocketLinux();

    /// Returns true if the object is ready for business
    virtual bool is_open() const
    {
        return m_fd != INVALID_SOCKET;
    }

    /// Close the socket
    virtual void close();

    /// Get the local socket address
    virtual Address const &local_address() const;

    /// Get the default destination address
    virtual Address const &destination_address() const;

    /// Send the packet referenced by pkt.
    virtual void send( PacketPtr const &pkt );

    /// Attempt to receive a packet from the network and store it in pkt.
    virtual PacketPtr recv();

    /// Join the specified multicast address
    virtual bool join_multicast( const char *interface_name, Address const &address );

    /// get the current file descriptor of the socket
    virtual socket_fd_t fd() const
    {
        return m_fd;
    }

    virtual void tick( Timestamp );
};

/// A shared ptr to a RawSocketLinux
typedef shared_ptr<RawSocketLinux> RawSocketLinuxPtr;

/// A vector of RawSockets
typedef std::vector<RawSocketLinuxPtr> RawSocketLinuxs;
}
}
