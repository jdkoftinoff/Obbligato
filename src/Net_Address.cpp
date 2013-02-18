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
#include "Obbligato/IOStream.hpp"
#include "Obbligato/Net_Address.hpp"
#include "Obbligato/Net_MAC48.hpp"

namespace Obbligato
{
    namespace Net
    {
        Address::Address()
        {
            memset(&m_storage,0,sizeof(m_storage));
            m_storage.ss_family = AF_UNSPEC;
        }

        Address::Address( MAC48 const &v )
        {
            memset(&m_storage,0,sizeof(m_storage));
#if defined(__linux__)
            sockaddr_ll *ll = (sockaddr_ll *)&m_storage;
            ll->sll_family = AF_PACKET;
            ll->sll_ifindex = 0;
            ll->sll_hatype = 0;
            ll->sll_pkttype = 0;
            ll->sll_halen = 6;
            ll->sll_addr[0] = v.m_value[0];
            ll->sll_addr[1] = v.m_value[1];
            ll->sll_addr[2] = v.m_value[2];
            ll->sll_addr[3] = v.m_value[3];
            ll->sll_addr[4] = v.m_value[4];
            ll->sll_addr[5] = v.m_value[5];
#elif defined(__APPLE__)
            sockaddr_dl *dl = (sockaddr_dl *)&m_storage;
            dl->sdl_len = sizeof(sockaddr_dl);
            dl->sdl_family = AF_LINK;
            dl->sdl_index = 0; // interface index (none)
            dl->sdl_type = 0;  // interface type
            dl->sdl_nlen = 0;  // interface name length (none)
            dl->sdl_alen = 6;  // address length is 6 for Mac48
            dl->sdl_slen = 0;  // link layer selector length (none)
            dl->sdl_data[0] = v.m_value[0];
            dl->sdl_data[1] = v.m_value[1];
            dl->sdl_data[2] = v.m_value[2];
            dl->sdl_data[3] = v.m_value[3];
            dl->sdl_data[4] = v.m_value[4];
            dl->sdl_data[5] = v.m_value[5];
#elif defined(WIN32) && defined(AF_LINK)
            sockaddr_dl *dl = (sockaddr_dl *)&m_storage;
            dl->sdl_family = AF_LINK;
            dl->sdl_data[0] = v.m_value[0];
            dl->sdl_data[1] = v.m_value[1];
            dl->sdl_data[2] = v.m_value[2];
            dl->sdl_data[3] = v.m_value[3];
            dl->sdl_data[4] = v.m_value[4];
            dl->sdl_data[5] = v.m_value[5];
            dl->sdl_data[6] = 0;
            dl->sdl_data[7] = 0;
#else
#error please define MAC48 constructor for platform specific sockaddr
#endif
        }


        Address::Address( sockaddr_storage const &addr ) :
            m_storage( addr )
        {
        }


        Address::Address( addrinfo const *ai )
        {
            memset(&m_storage,0,sizeof(m_storage));

            socklen_t l = get_addrlen(ai->ai_addr);
            if( l>0 )
            {
                memcpy( &m_storage, ai->ai_addr, l );
            }
            else
            {
                m_storage.ss_family = AF_UNSPEC;
            }
        }

        Address::Address( sockaddr const *addr )
        {
            memset(&m_storage,0,sizeof(m_storage));

            socklen_t l = get_addrlen(addr);
            if( l>0 )
            {
                memcpy( &m_storage, addr, l );
            }
            else
            {
                m_storage.ss_family = AF_UNSPEC;
            }
        }

#if defined(__linux__)
        Address::Address( sockaddr_ll const *addr )
        {
            memset(&m_storage,0,sizeof(m_storage));
            memcpy(&m_storage,addr,sizeof(sockaddr_ll));
        }

#elif defined(__APPLE__)
        Address::Address( sockaddr_dl const *addr )
        {
            memset(&m_storage,0,sizeof(m_storage));
            memcpy(&m_storage,addr,sizeof(sockaddr_dl));
        }
#elif defined(WIN32) && defined(AF_LINK)
        Address::Address( sockaddr_dl const *addr )
        {
            memset(&m_storage,0,sizeof(m_storage));
            memcpy(&m_storage,addr,sizeof(sockaddr_dl));
        }
#endif

        Address::Address( sockaddr_in const *addr )
        {
            memset(&m_storage,0,sizeof(m_storage));
            memcpy(&m_storage,addr,sizeof(sockaddr_in));
        }

        Address::Address( sockaddr_in6 const *addr )
        {
            memset(&m_storage,0,sizeof(m_storage));
            memcpy(&m_storage,addr,sizeof(sockaddr_in6));
        }

        Address::Address( std::string addrstring, int family )
        {
            memset(&m_storage,0,sizeof(m_storage));

            m_storage.ss_family = family;
            from_string(addrstring,family);
        }

        bool Address::is_mac48() const
        {
            bool r=false;
#if defined(__linux__)
            if( m_storage.ss_family == AF_PACKET )
            {
                sockaddr_ll const *ll = (sockaddr_ll const *)&m_storage;
                if( ll->sll_halen == 6)
                {
                    r=true;
                }
            }
#elif defined(__APPLE__)
            if( m_storage.ss_family == AF_LINK )
            {
                sockaddr_dl const *dl = (sockaddr_dl const *)&m_storage;
                if( dl->sdl_alen == 6 )
                {
                    r=true;
                }
            }
#elif defined(WIN32) && defined(AF_LINK)
            if( m_storage.ss_family == AF_LINK )
            {
                r=true;
            }
#else
# error Please define MAC48 address handler
#endif
            return r;
        }

        MAC48 Address::get_mac48() const
        {
            IEEE::MAC48 r;
#if defined(__linux__)
            if( m_storage.ss_family == AF_PACKET )
            {
                sockaddr_ll const *ll = (sockaddr_ll const *)&m_storage;
                if( ll->sll_halen == 6)
                {
                    r.m_value[0] = ll->sll_addr[0];
                    r.m_value[1] = ll->sll_addr[1];
                    r.m_value[2] = ll->sll_addr[2];
                    r.m_value[3] = ll->sll_addr[3];
                    r.m_value[4] = ll->sll_addr[4];
                    r.m_value[5] = ll->sll_addr[5];
                }
            }
#elif defined(__APPLE__)
            if( m_storage.ss_family == AF_LINK )
            {
                sockaddr_dl const *dl = (sockaddr_dl const *)&m_storage;

                if( dl->sdl_alen == 6 )
                {
                    uint8_t *p = (uint8_t*)LLADDR(dl);
                    r.m_value[0] = p[0];
                    r.m_value[1] = p[1];
                    r.m_value[2] = p[2];
                    r.m_value[3] = p[3];
                    r.m_value[4] = p[4];
                    r.m_value[5] = p[5];
                }
            }
#elif defined(WIN32) && defined(AF_LINK)
            if( m_storage.ss_family == AF_LINK )
            {
                sockaddr_dl const *dl = (sockaddr_dl const *)&m_storage;

                r.m_value[0] = dl->sdl_data[0];
                r.m_value[1] = dl->sdl_data[1];
                r.m_value[2] = dl->sdl_data[2];
                r.m_value[3] = dl->sdl_data[3];
                r.m_value[4] = dl->sdl_data[4];
                r.m_value[5] = dl->sdl_data[5];
            }
#else
# error Please define MAC48 address handler
#endif
            return r;
        }


        bool Address::is_ipv4() const
        {
            return m_storage.ss_family == AF_INET;
        }

        sockaddr_in Address::get_ipv4() const
        {
            sockaddr_in const *sa = (sockaddr_in const *)&m_storage;
            return *sa;
        }

        bool Address::is_ipv6() const
        {
            return m_storage.ss_family == AF_INET6;
        }

        sockaddr_in6 Address::get_ipv6() const
        {
            sockaddr_in6 const *sa = (sockaddr_in6 const *)&m_storage;
            return *sa;
        }


        socklen_t Address::get_addrlen(sockaddr const *sa)
        {
            socklen_t r=0;
            switch (sa->sa_family)
            {
                case AF_INET:
                    r=sizeof(struct sockaddr_in);
                    break;
                case AF_INET6:
                    r=sizeof(sockaddr_in6);
                    break;
#if defined(__linux__)
                case AF_PACKET:
                    r=sizeof(struct sockaddr_ll);
                    break;
#elif defined(__APPLE__)
                case AF_LINK:
                    r=sizeof(struct sockaddr_dl);
                    break;
#elif defined(WIN32) && defined(AF_LINK)
                case AF_LINK:
                    r=sizeof(struct sockaddr_dl);
                    break;
#else
# error please define get_addrlen for platform ethernet address
#endif
                default:
                    r=-1;
            }
            return r;
        }


        std::string Address::to_string() const
        {
            char buf[1024]="";
            socklen_t l=get_addrlen();
            std::string r;

            if( l>0 )
            {
                if( is_mac48() )
                {
                    MAC48 m = get_mac48();
                    r = m.to_string();
                }
                else if( is_ipv4() || is_ipv6() )
                {
                    if( getnameinfo((sockaddr const *)&m_storage, l, buf, sizeof(buf), nullptr, 0, NI_NUMERICHOST )==0 )
                    {
                        r = buf;
                    }
                    else
                    {
                        throw std::invalid_argument("Bad parameter to getnameinfo");
                    }
                }
                else
                {
                    r = "";
                }
            }

            return r;
        }

        void Address::from_string( std::string const &s, int family )
        {
            switch( family )
            {
                case AF_UNSPEC:
                    if( from_string_mac48(s) )
                        return;
                    if( from_string_ipv4(s) )
                        return;
                    if( from_string_ipv6(s) )
                        return;
                    break;
#if defined(__linux__)
                case AF_PACKET:
                    if( from_string_mac48(s) )
                        return;
                    break;
#elif defined(__APPLE__)
                case AF_LINK:
                    if( from_string_mac48(s) )
                        return;
                    break;
#elif defined(WIN32) && defined(AF_LINK)
                case AF_LINK:
                    if( from_string_mac48(s) )
                        return;
                    break;
#else
# error Please add platform specific from_string for mac address
#endif
                case AF_INET:
                    if( from_string_ipv4(s) )
                        return;
                    break;
                case AF_INET6:
                    if( from_string_ipv6(s) )
                        return;
                    break;
            }

            throw std::invalid_argument("Bad parameter to Address:from_string)");
        }

        bool Address::from_string_mac48( std::string const &s )
        {
            bool r=false;

            MAC48 m;
            if( m.from_string(s) )
            {
                r=true;
                assign(Address(m));
            }

            return r;
        }

        bool Address::from_string_ipv4( std::string const &s )
        {
            bool r=false;
            int e=0;
            addrinfo *ai=0;
            addrinfo hints;
            memset(&hints,0,sizeof(hints));

            hints.ai_family = AF_INET;
            hints.ai_flags = AI_NUMERICHOST;

            e=getaddrinfo(s.c_str(),0,&hints,&ai);
            if( e==0 && ai )
            {
                memcpy( &m_storage, ai->ai_addr, ai->ai_addrlen );
                r=true;
            }
            if( ai )
            {
                freeaddrinfo(ai);
            }
            return r;
        }

        bool Address::from_string_ipv6( std::string const &s )
        {
            bool r=false;
            int e=0;
            addrinfo *ai=0;
            addrinfo hints;
            memset(&hints,0,sizeof(hints));

            hints.ai_family = AF_INET6;
            hints.ai_flags = AI_NUMERICHOST;

            e=getaddrinfo(s.c_str(),0,&hints,&ai);
            if( e==0 && ai )
            {
                memcpy( &m_storage, ai->ai_addr, ai->ai_addrlen );
                r=true;
            }
            if( ai )
            {
                freeaddrinfo(ai);
            }
            return r;
        }

    }

    void from_string( Net::Address &to, std::string &from )
    {
        to.from_string(from,AF_UNSPEC);
    }

    void to_string( std::string &to, Net::Address const &v )
    {
        to_string( to, v.get_sockaddr() );
    }

    std::ostream & operator <<(std::ostream& o, Net::Address const &v)
    {
        o << v.to_string();
        return o;
    }

    std::istream & operator >>(std::istream& i, Net::Address &v)
    {
        std::string s;
        i >> s;
        v.from_string(s);
        return i;
    }

}

