#pragma once
#ifndef Obbligato_Net_LinkStatusBase_hpp
#define Obbligato_Net_LinkStatusBase_hpp

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
#include "Obbligato/Net_MAC48.hpp"

namespace Obbligato
{
    namespace Net
    {

        /** \addtogroup linkstatus
         */
        /*@{*/

        /// Abstract base class for object which can get the link status of an ethernet port
        class LinkStatusBase
        {
        public:
            LinkStatusBase() {}
            virtual ~LinkStatusBase() {}

            virtual bool get_link_status( const char *devname ) = 0;
        };


        /// Concrete implementation of LinkStatusChecker, always returns link status up
        class NetLinkStatusNull : public LinkStatusBase
        {
        public:
            bool get_link_status( const char * )
            {
                return true; // assume link is up
            }
        };

        /*@}*/

    }
}

#endif


