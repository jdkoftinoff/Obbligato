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
#include "Obbligato/Net_Socket.hpp"

namespace Obbligato
{
    namespace Net
    {
        Address get_local_address( SOCKET fd )
        {
            sockaddr_storage addr;
            socklen_t addr_len = sizeof( addr );

            if( ::getsockname( fd, (sockaddr *)&addr, &addr_len )!=0 )
            {
                throw std::invalid_argument( "get_local_address");
            }
            return Address(addr);
        }

        Address get_remote_address( SOCKET fd )
        {
            sockaddr_storage addr;
            socklen_t addr_len = sizeof( addr );

            if( ::getpeername( fd, (sockaddr *)&addr, &addr_len )!=0 )
            {
                throw std::invalid_argument( "get_remote_address");
            }
            return Address(addr);
        }
    }
}

