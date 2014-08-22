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
#include "Obbligato/SharedPtr.hpp"
#include "Obbligato/Logger.hpp"

namespace Obbligato
{
namespace Net
{

typedef shared_ptr<addrinfo> SharedAddrInfo;

class AddrInfoDeleter : public DeleterBase<addrinfo>
{
  public:
    void operator()( addrinfo const *p ) const
    {
        ::freeaddrinfo( const_cast<addrinfo *>( p ) );
    }
};

SharedAddrInfo GetAddrInfo( std::string const &hostname, std::string const &port, int family, int socktype, int flags );

SharedAddrInfo
    GetAddrInfoForTcpServer( std::string const &hostname, std::string const &port, bool do_dns_lookup, bool do_port_lookup );

SharedAddrInfo
    GetAddrInfoForUdpServer( std::string const &hostname, std::string const &port, bool do_dns_lookup, bool do_port_lookup );

SharedAddrInfo
    GetAddrInfoForTcp( std::string const &hostname, std::string const &port, bool do_dns_lookup, bool do_port_lookup );

SharedAddrInfo
    GetAddrInfoForUdp( std::string const &hostname, std::string const &port, bool do_dns_lookup, bool do_port_lookup );

typedef std::vector<Address> AddressList;

AddressList make_addresslist( SharedAddrInfo const &ai );
AddressList make_addresslist( addrinfo const *ai );

std::istream &operator>>( std::istream &i, AddressList &v );
std::ostream &operator<<( std::ostream &o, AddressList const &v );
}
}
