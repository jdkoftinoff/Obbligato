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
#include "Obbligato/Net_AddressList.hpp"

namespace Obbligato
{
    namespace Net
    {
        SharedAddrInfo GetAddrInfo(
                std::string const &hostname,
                std::string const &port,
                int family,
                int socktype,
                int flags
                )
        {
            ::addrinfo *ai=0;
            ::addrinfo hints;
            ::memset(&hints, '\0', sizeof(hints));
            hints.ai_socktype = socktype;
            hints.ai_family = family;
            hints.ai_flags = flags;
            int e = ::getaddrinfo(
                hostname.length()>0 ? hostname.c_str() : 0,
                port.length()>0 ? port.c_str() : 0,
                &hints,
                &ai
                );
            if (e != 0)
            {
                throw_gai_error(e);
            }

            return SharedAddrInfo(ai,AddrInfoDeleter());
        }

        SharedAddrInfo GetAddrInfoForTcpServer(
                std::string const &hostname,
                std::string const &port,
                bool do_dns_lookup,
                bool do_port_lookup
                )
        {
            int a = do_dns_lookup ? 0 : AI_NUMERICHOST;
            int b = do_port_lookup ? 0 : AI_NUMERICSERV;

            return GetAddrInfo( hostname, port, AF_UNSPEC, SOCK_STREAM, AI_ADDRCONFIG | AI_PASSIVE | a | b );
        }

        SharedAddrInfo GetAddrInfoForUdpServer(
                std::string const &hostname,
                std::string const &port,
                bool do_dns_lookup,
                bool do_port_lookup
                )
        {
            int a = do_dns_lookup ? 0 : AI_NUMERICHOST;
            int b = do_port_lookup ? 0 : AI_NUMERICSERV;

            return GetAddrInfo( hostname, port, AF_UNSPEC, SOCK_DGRAM, AI_ADDRCONFIG | AI_PASSIVE | a | b );
        }

        SharedAddrInfo GetAddrInfoForTcp(
                std::string const &hostname,
                std::string const &port,
                bool do_dns_lookup,
                bool do_port_lookup
                )
        {
            int a = do_dns_lookup ? 0 : AI_NUMERICHOST;
            int b = do_port_lookup ? 0 : AI_NUMERICSERV;

            return GetAddrInfo( hostname, port, AF_UNSPEC, SOCK_STREAM, AI_ADDRCONFIG | a | b );
        }

        SharedAddrInfo GetAddrInfoForUdp(
                std::string const &hostname,
                std::string const &port,
                bool do_dns_lookup,
                bool do_port_lookup
                )
        {
            int a = do_dns_lookup ? 0 : AI_NUMERICHOST;
            int b = do_port_lookup ? 0 : AI_NUMERICSERV;

            return GetAddrInfo( hostname, port, AF_UNSPEC, SOCK_DGRAM, AI_ADDRCONFIG | a | b );
        }

        NetAddressList::NetAddressList(
                SharedAddrInfo const &ai
                )
        {
            assign(ai);
        }

        void NetAddressList::assign(const SharedAddrInfo &ai)
        {
            assign( ai.get() );
        }

        void NetAddressList::assign(const addrinfo *ai)
        {
            m_addresses.clear();
            while( ai )
            {
                m_addresses.push_back(NetAddress(ai));
                ai=ai->ai_next;
            }
        }

        std::istream & operator >>(std::istream& i, Net::NetAddressList &v )
        {
            NetAddressList r;

            std::string t;
            i >> t;
            if( t!="{")
            {
                throw std::invalid_argument("expected '{'");
            }
            while( i>>t )
            {
                if( t=="}" )
                {
                    break;
                }
                else
                {
                    NetAddress a;
                    a.from_string(t);
                    r.push_back(a);
                }
            };
            v = r;
            return i;
        }

        std::ostream & operator <<(std::ostream& o, Net::NetAddressList const &v)
        {
            std::string r;
            r.clear();
            r.append("{ ");

            for (Net::NetAddressList::const_iterator i = v.begin(); i != v.end(); ++i)
            {
                r.append( i->to_string() );
                r.push_back(' ');
            }
            r.push_back('}');
            o << r;
            return o;
        }

    }


}

