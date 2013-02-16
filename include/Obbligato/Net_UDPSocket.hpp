#pragma once
#ifndef Obbligato_Net_UDPSocket_hpp
#define Obbligato_Net_UDPSocket_hpp

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

namespace Obbligato
{
    namespace Net
    {
        class NetUDPSocket : public NetSocket
        {
        public:

            NetUDPSocket() {}

            /// Close and destroy the socket
            virtual ~NetUDPSocket();

            /// Returns true if the object is ready for business
            virtual bool is_open() const = 0;

            /// Close the socket
            virtual void close() = 0;

            /// Get the local socket address
            virtual NetAddress local_address() = 0;

            /// Get the default destination address
            virtual NetAddress destination_address() = 0;

            /// Send the packet referenced by pkt.
            virtual ssize_t send( NetPacket const &pkt ) = 0;

            /// Attempt to receive a packet from the network and store it in pkt.
            /**
             *  On successful receive, all pkt fields are filled in.
             *
             *  Returns the number of bytes received.
             */
            virtual ssize_t recv( NetPacket &pkt ) = 0;

            /// Join the specified multicast MAC address
            virtual bool join_multicast(
                    const char *interface_name,
                    int address_family,
                    struct sockaddr *mcast_addr,
                    socklen_t mcast_addr_len
                    ) = 0;

            /// get the current file descriptor of the socket
            virtual SOCKET fd() const = 0;
        };

        typedef std::vector< shared_ptr< NetUDPSocket > > NetUDPSockets;

    }
}

#endif


