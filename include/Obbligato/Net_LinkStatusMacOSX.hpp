#pragma once
#ifndef Obbligato_Net_LinkStatusMacOSX_hpp
#define Obbligato_Net_LinkStatusMacOSX_hpp

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

#include "Obbligato/Net_LinkStatusBase.hpp"

#if defined(__APPLE__)

namespace Obbligato
{
    namespace Net
    {
        /** \addtogroup linkstatus
         */
        /*@{*/

        /// Mac OS X Specific link status checker.
        class LinkStatusMacOSX : public LinkStatusBase
        {
            int fd;

            bool reopen()
            {
                if( fd==-1 )
                {
                    fd = socket( AF_INET, SOCK_DGRAM, 0 );
                }
                return fd>=0;
            }

        public:
            LinkStatusMacOSX()  : fd(-1)
            {
            }

            ~LinkStatusMacOSX()
            {
                if( fd!=-1 )
                {
                    close(fd);
                }
            }

            bool get_link_status( const char *eth );

        };

        typedef LinkStatusMacOSX LinkStatusDefault;

        /*@}*/

    }
}

#endif


#endif


