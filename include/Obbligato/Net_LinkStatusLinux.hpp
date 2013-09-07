#pragma once
#ifndef Obbligato_Net_LinkStatusLinux_hpp
#define Obbligato_Net_LinkStatusLinux_hpp

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
#include "Obbligato/Net_LinkStatusBase.hpp"

namespace Obbligato {
namespace Net {

/** \addtogroup linkstatus
         */
/*@{*/

#if defined(__linux__)

/// Linux specific link status checker. Opens a socket and uses ioctl to poll
/// ethernet port link status
class LinkStatusLinux : public LinkStatusBase {
    int fd;

    bool reopen() {
        if (fd == -1) {
            fd = socket(AF_INET, SOCK_DGRAM, 0);
        }
        return fd >= 0;
    }

  public:
    LinkStatusLinux() : fd(-1) {}

    ~LinkStatusLinux() {
        if (fd != -1) {
            ::close(fd);
        }
    }

    bool get_link_status(const char *eth);
};

typedef LinkStatusLinux LinkStatusDefault;
#endif

/*@}*/
}
}

#endif
