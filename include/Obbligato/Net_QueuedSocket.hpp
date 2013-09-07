#pragma once
#ifndef Obbligato_Net_QueuedSocket_hpp
#define Obbligato_Net_QueuedSocket_hpp

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
#include "Obbligato/Net_RawSocket.hpp"

namespace Obbligato {
namespace Net {

/// Base class for socket with with Incoming and Outgoing packet queues
template <typename QueueT, typename SocketT> class QueuedSocket {
  public:
    typedef QueueT queue_type;
    typedef SocketT socket_type;
    typedef typename QueueT::value_type packet_type;

  private:
    queue_type m_incoming_queue;
    queue_type m_outgoing_queue;
    socket_type *m_sock;

  public:

    /// Create a queued rawnet socket with incoming, outgoing, and raw socket
    QueuedSocket(socket_type *sock) : m_sock(sock) {}

    virtual ~QueuedSocket() { delete m_sock; }

    /// Get a reference to the incoming queue
    queue_type &incoming_queue() { return m_incoming_queue; }

    /// Get a const reference to the incoming queue
    queue_type const &incoming_queue() const { return m_incoming_queue; }

    /// Get a reference to the outgoing queue
    queue_type &outgoing_queue() { return m_outgoing_queue; }

    /// Get a const reference to the outgoing queue
    queue_type const &outgoing_queue() const { return m_outgoing_queue; }

    /// Check if there is any data in the outgoing_queue to be sent over the
    /// socket
    bool outgoing_not_empty() const { return !m_outgoing_queue.empty(); }

    /// Check if there is any space in the outgoing_queue
    bool outgoing_not_full() const { return !m_outgoing_queue.full(); }

    /// Check if there is any data in the incoming_queue
    bool incoming_not_empty() const { return !m_incoming_queue.empty(); }

    /// Check if there is any space in the incoming_queue for received packets
    bool incoming_not_full() const { return !m_incoming_queue.full(); }

    /// Place a packet into the outgoing queue
    bool send(packet_type const &outpkt) {
        bool r = false;
        if (outgoing_not_empty()) {
            m_outgoing_queue.incoming() = outpkt;
            m_outgoing_queue.push();
            r = true;
        }
        return r;
    }

    /// Try receive a packet from the incoming queue
    bool recv(packet_type &inpkt) {
        bool r = false;
        if (incoming_not_empty()) {
            inpkt = m_incoming_queue.outgoing();
            m_incoming_queue.pop();
            r = true;
        }
        return r;
    }

    /// Send one packet from the outgoing_queue out the socket
    bool send_one_from_queue_to_socket() {
        bool r = false;
        // Is there a packet to send?
        if (outgoing_not_empty()) {
            // Yes, try send it
            if (m_sock->send(outgoing_queue().outgoing()) >= 0) {
                // Success sending it, so pop the packet off the outgoing queue
                outgoing_queue().pop();

                // and show success
                r = true;
            }
        }
        return r;
    }

    /// Try receive one packet from the socket into the incoming_queue
    bool receive_one_from_socket_into_queue() {
        bool r = false;
        // Is there space to receive a packet?
        if (incoming_not_full()) {
            // Yes, try receive a packet into the queue
            if (m_sock->recv(incoming_queue().incoming()) >= 0) {
                // Got one! push it into the incoming queue
                incoming_queue().push();

                // and show success
                r = true;
            }
        }
        return r;
    }

    /// Get a reference to the RawNetSocket
    socket_type &sock() { return *m_sock; }

    /// Get a const reference to the RawNetSocket
    socket_type const &sock() const { return *m_sock; }
};
}
}

#endif
