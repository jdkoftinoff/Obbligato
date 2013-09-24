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
#include "Obbligato/Net_Socket.hpp"

namespace Obbligato {
namespace Net {

/// Class for socket with with Incoming and Outgoing packet queues
template <typename ItemType, typename SocketT> class QueuedSocket {
  public:
    typedef ItemType item_type;
    typedef std::queue<ItemType> queue_type;
    typedef SocketT socket_type;
    typedef QueuedSocket<ItemType, SocketT> queuedsocket_type;

    QueuedSocket(socket_type &&s) : sock(std::move(s)) {}

    QueuedSocket(QueuedSocket &&other)
        : sock(std::move(other.sock)),
          incoming_queue(std::move(other.incoming_queue)),
          outgoing_queue(std::move(other.outgoing_queue)) {}

    QueuedSocket const &operator=(QueuedSocket &&other) {
        sock = std::move(other.sock);
        incoming_queue = std::move(other.incoming_queue);
        outgoing_queue = std::move(other.outgoing_queue);
        return *this;
    }

    socket_type sock;
    queue_type incoming_queue;
    queue_type outgoing_queue;
};

template <typename ItemType, typename SocketT>
inline QueuedSocket<ItemType, SocketT> make_queuedsocket(SocketT &&sock) {
    return QueuedSocket<ItemType, SocketT>(std::move(sock));
}
}
}
