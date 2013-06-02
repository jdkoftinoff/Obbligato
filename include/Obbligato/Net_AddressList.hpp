#pragma once
#ifndef Obbligato_Net_AddressList_hpp
#define Obbligato_Net_AddressList_hpp

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

namespace Obbligato { namespace Net {

typedef shared_ptr< addrinfo > SharedAddrInfo;

class AddrInfoDeleter : public DeleterBase< addrinfo >
{
public:
    void operator () ( addrinfo const *p ) const
    {
        ::freeaddrinfo( const_cast<addrinfo *>(p) );
    }
};


SharedAddrInfo GetAddrInfo(
        std::string const &hostname,
        std::string const &port,
        int family,
        int socktype,
        int flags
        );

SharedAddrInfo GetAddrInfoForTcpServer(
        std::string const &hostname,
        std::string const &port,
        bool do_dns_lookup,
        bool do_port_lookup
        );

SharedAddrInfo GetAddrInfoForUdpServer(
        std::string const &hostname,
        std::string const &port,
        bool do_dns_lookup,
        bool do_port_lookup
        );

SharedAddrInfo GetAddrInfoForTcp(
        std::string const &hostname,
        std::string const &port,
        bool do_dns_lookup,
        bool do_port_lookup
        );

SharedAddrInfo GetAddrInfoForUdp(
        std::string const &hostname,
        std::string const &port,
        bool do_dns_lookup,
        bool do_port_lookup
        );

class AddressList
{
    typedef std::vector<Address> container_type;
    container_type m_addresses;
public:

    typedef container_type::const_iterator const_iterator;
    typedef container_type::const_pointer const_pointer;
    typedef container_type::const_reference const_reference;
    typedef container_type::const_reverse_iterator const_reverse_iterator;
    typedef container_type::iterator iterator;
    typedef container_type::pointer pointer;
    typedef container_type::reference reference;
    typedef container_type::reverse_iterator reverse_iterator;
    typedef container_type::size_type size_type;

    const_iterator begin() const { return m_addresses.begin(); }
    iterator begin() { return m_addresses.begin(); }
    const_reverse_iterator rbegin() const { return m_addresses.rbegin(); }
    reverse_iterator rbegin() { return m_addresses.rbegin(); }

    const_iterator end() const { return m_addresses.end(); }
    iterator end() { return m_addresses.end(); }
    const_reverse_iterator rend() const { return m_addresses.rend(); }
    reverse_iterator rend() { return m_addresses.rend(); }

    void clear() { m_addresses.clear(); }
    size_type size() const { return m_addresses.size(); }

    void push_back( Address const &a ) { return m_addresses.push_back(a); }

    AddressList()
    {
    }

    AddressList( SharedAddrInfo const &ai );

    AddressList( addrinfo const *ai );

    AddressList( AddressList const &other )
        : m_addresses( other.m_addresses )
    {
    }

    AddressList const & operator = ( AddressList const &other )
    {
        assign( other );
        return *this;
    }

    void assign( AddressList const &other )
    {
        m_addresses = other.m_addresses;
    }

    void assign( SharedAddrInfo const &ai );

    void assign( addrinfo const *ai );

    void swap( AddressList &other )
    {
        m_addresses.swap( other.m_addresses );
    }

    virtual ~AddressList()
    {
    }

    friend std::istream & operator >>(std::istream& i, Net::AddressList &v );

    friend std::ostream & operator <<(std::ostream& o, Net::AddressList const &v);
};

}}

#endif


