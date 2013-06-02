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
#include "Obbligato/Net_LinkStatusLinux.hpp"

#if defined(__linux__)
// Linux specific includes for raw network access and link status
# include <sys/ioctl.h>
# include <linux/ethtool.h>
# include <linux/sockios.h>


namespace Obbligato { namespace Net {

bool LinkStatusLinux::get_link_status( const char *eth )
{
    bool r=false;

    struct ifreq ifr;
    struct ethtool_value edata;

    memset(&edata, 0, sizeof(edata));
    memset(&ifr, 0, sizeof(ifr));
    edata.cmd = ETHTOOL_GLINK;
    strcpy( ifr.ifr_name,eth);
    ifr.ifr_data = (char *)&edata;

    if( ::ioctl( fd, SIOCETHTOOL, &ifr  )==0 )
    {
        if( edata.data !=0 )
        {
            r=true;
        }
    }
    return r;
}

}}

#endif

