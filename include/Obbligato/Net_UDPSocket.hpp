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
        class UDPSocket : public Socket
        {
        private:
            SOCKET m_fd;

            Address m_local_addr;
            Address m_default_dest_addr;

        public:

            UDPSocket(
                    Address local_addr,
                    Address default_dest_addr
                    );

            /// Close and destroy the socket
            virtual ~UDPSocket();

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
            virtual Address const & destination_address() const;

            /// Send the packet referenced by pkt.
            virtual ssize_t send( Packet const &pkt );

            /// Attempt to receive a packet from the network and store it in pkt.
            /**
             *  On successful receive, all pkt fields are filled in.
             *
             *  Returns the number of bytes received.
             */
            virtual ssize_t recv( Packet &pkt );

            /// Join the specified multicast address
            virtual bool join_multicast(
                    const char *interface_name,
                    Address const &address
                    );

            /// get the current file descriptor of the socket
            virtual SOCKET fd() const
            {
                return m_fd;
            }
        };


        /// A shared ptr to a UDPSocket
        typedef shared_ptr< UDPSocket > UDPSocketPtr;

        /// A vector of UDPSockets
        typedef std::vector< UDPSocketPtr > UDPSockets;
    }
}

#endif


