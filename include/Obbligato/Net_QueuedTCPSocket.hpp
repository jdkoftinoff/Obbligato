#pragma once
#ifndef Obbligato_Net_QueuedTCPSocket_hpp
#define Obbligato_Net_QueuedTCPSocket_hpp

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
#include "Obbligato/Queue.hpp"
#include "Obbligato/Net_QueuedSocket.hpp"
#include "Obbligato/Net_TCPSocket.hpp"

namespace Obbligato { namespace Net {

typedef QueuedSocket< Queue<uint8_t>, TCPSocket> QueuedTCPSocket;

typedef shared_ptr< QueuedTCPSocket > QueuedTCPSocketPtr;

inline QueuedTCPSocketPtr create_queued_tcp_socket(
        size_t queue_size,
        Address local_address
        )
{
    return QueuedTCPSocketPtr(
                new QueuedTCPSocket(
                    queue_size,
                    new TCPSocket( local_address )
                    )
                );
}


typedef std::vector< QueuedTCPSocketPtr > QueuedTCPSockets;

}}

#endif


