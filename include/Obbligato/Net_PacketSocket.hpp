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
#include "Obbligato/Net_Address.hpp"
#include "Obbligato/Net_Socket.hpp"
#include "Obbligato/Net_Packet.hpp"

namespace Obbligato {
namespace Net {

class PacketSocket : public Socket {
  public:

    /// Close and destroy the socket
    virtual ~PacketSocket() {}

    /// Send the packet referenced by pkt.
    virtual void send(Packet const &pkt) = 0;

    /// Attempt to receive a packet from the network
    virtual Packet recv() = 0;

    /// Join the specified multicast address
    virtual bool join_multicast(const char *interface_name,
                                Address const &address) = 0;

    virtual void tick(Timestamp) = 0;
};

/// A shared ptr to a UDPSocket
typedef shared_ptr<PacketSocket> PacketSocketPtr;

/// A vector of UDPSockets
typedef std::vector<PacketSocketPtr> PacketSockets;

inline socket_fd_t get_fd(PacketSocketPtr const &s) { return s->fd(); }
}
}
