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
#include "Obbligato/Net.hpp"
#include "Obbligato/Net_UDPSocket.hpp"

namespace Obbligato { namespace Net {

UDPSocket::UDPSocket(
        Address local_addr,
        Address default_dest_addr
        )
    : m_local_addr( local_addr ),
      m_default_dest_addr( default_dest_addr )
{
    initialize_sockets();

    do
    {
        m_fd = ::socket (
                    m_local_addr.get_sa_family(),
                    SOCK_DGRAM,
                    0
                    );
    } while( m_fd<0 && (errno==EAGAIN || errno==EINTR) );

    if( m_fd!=INVALID_SOCKET )
    {
        int on = 1;
        if ( ::setsockopt ( m_fd, SOL_SOCKET, SO_REUSEADDR, ( const char * ) &on, sizeof ( on ) ) == 0 )
        {
            if ( ::bind ( m_fd, m_local_addr.get_sockaddr(), m_local_addr.get_addrlen() ) == 0 )
            {
                try
                {
                    set_socket_nonblocking( m_fd );

                    m_local_addr = get_local_address( m_fd );
                }
                catch(...)
                {
                    close();
                    throw;
                }
            }
            else
            {
                close();
                throw std::runtime_error( "Unable to bind socket" );
            }
        }
        else
        {
            close();
            throw std::runtime_error( "Unable to setsockopt SO_REUSEADDR" );
        }
    }
    else
    {
        throw std::runtime_error( "Unable to create socket" );
    }
}



UDPSocket::~UDPSocket()
{
#ifdef _WIN32
    ::closesocket(m_fd);
#else
    ::close(m_fd);
#endif
    m_fd = INVALID_SOCKET;
}

void UDPSocket::close()
{
#ifdef _WIN32
    ::closesocket(m_fd);
#else
    ::close(m_fd);
#endif
    m_fd = INVALID_SOCKET;
}

Address const &UDPSocket::local_address() const
{
    return m_local_addr;
}

Address const &UDPSocket::destination_address() const
{
    return m_default_dest_addr;
}

ssize_t UDPSocket::send( Packet const &pkt )
{
    ssize_t r=-1;
    sockaddr const *dest_addr = m_default_dest_addr.get_sockaddr();
    socklen_t dest_addr_len = m_default_dest_addr.get_addrlen();

    if( pkt.destination_address().get_addrlen()>0 )
    {
        dest_addr = pkt.destination_address().get_sockaddr();
        dest_addr_len = pkt.destination_address().get_addrlen();
    }

    do
    {
        r = ::sendto(
                    m_fd,
                    (const char *)pkt.payload().data.data(),
                    pkt.payload().data.size(),
                    0,
                    dest_addr,
                    dest_addr_len
                    );

    } while( r<0 && (errno==EINTR) );

    return r;
}

ssize_t UDPSocket::recv( Packet &pkt )
{
    ssize_t r=-1;
    pkt.payload().data.size();
    sockaddr_storage addr;
    socklen_t addr_len = sizeof( addr );

    do
    {
        r = ::recvfrom(
                    m_fd,
                    (char *)pkt.payload().data.data(),
                    pkt.payload().data.capacity(),
                    0,
                    (struct sockaddr *)&addr,
                    &addr_len
                    );

    } while( r<0 && (errno==EINTR) );

    if( r>=0 )
    {
        pkt.timestamp( Time::get_current_timestamp() );
        pkt.source_address( Address(addr) );
        pkt.destination_address( Address() );
        pkt.network_port_address( m_local_addr );
        pkt.payload().data.size();
    }

    return r;
}

bool UDPSocket::join_multicast(
        const char *interface_name,
        Address const &mcast_addr
        )
{
    bool r=false;
    int if_index = if_nametoindex ( interface_name );
    if( m_local_addr.get_sa_family() != mcast_addr.get_sa_family() )
    {
        // wrong address type for sure!
        return false;
    }
    if ( mcast_addr.get_sa_family() == AF_INET6 )
    {
        struct ipv6_mreq multicast_request;
        multicast_request.ipv6mr_multiaddr = mcast_addr.get_ipv6().sin6_addr;
        multicast_request.ipv6mr_interface = if_index;
        if ( ::setsockopt (
                 m_fd,
                 IPPROTO_IPV6,
                 IPV6_JOIN_GROUP,
                 ( char* ) &multicast_request,
                 sizeof ( multicast_request ) ) == 0 )
        {
            r=true;
        }
    }
    else
    {
        if ( mcast_addr.get_sa_family() == AF_INET )
        {
            struct in_addr in_local;
            in_local.s_addr = mcast_addr.get_ipv4().sin_addr.s_addr;
            struct ip_mreq multicast_request;

            multicast_request.imr_multiaddr = mcast_addr.get_ipv4().sin_addr;
            multicast_request.imr_interface.s_addr = in_local.s_addr;
            if ( ::setsockopt (
                     m_fd,
                     IPPROTO_IP,
                     IP_ADD_MEMBERSHIP,
                     ( char* ) &multicast_request, sizeof ( multicast_request ) ) == 0
                 )
            {
                r=true;
            }
        }
    }

    return r;
}

}}

