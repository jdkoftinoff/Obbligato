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
#include "Obbligato/Net_LinkStatusMacOSX.hpp"

#if defined(__APPLE__) && (TARGET_OS_IPHONE == 0)

// Mac OS X specific includes for raw network access and link status
#include <sys/types.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <sys/sockio.h>
#include <sys/ioctl.h>
#include <net/if_media.h>

namespace Obbligato {
namespace Net {

bool LinkStatusMacOSX::get_link_status(const char *name) {
    bool r = false;

    struct ifmediareq ifmr;
    int *media_list;

    memset(&ifmr, 0, sizeof(ifmr));
    strncpy(ifmr.ifm_name, name, sizeof(ifmr.ifm_name));

    if (::ioctl(fd, SIOCGIFMEDIA, (caddr_t) & ifmr) >= 0) {
        if (ifmr.ifm_count > 0) {
            media_list = (int *)malloc(ifmr.ifm_count * sizeof(int));
            if (media_list != NULL) {
                ifmr.ifm_ulist = media_list;

                if (ioctl(fd, SIOCGIFMEDIA, (caddr_t) & ifmr) >= 0) {
                    if (ifmr.ifm_status & IFM_AVALID) {
                        if (ifmr.ifm_status & IFM_ACTIVE) {
                            r = true;
                        } else {
                            r = false;
                        }
                    }
                }
                free(media_list);
            }
        }
    } else {
        r = true; // Unable to do IOCTL, so assume link is up
    }

    return r;
}
}
}
#else
namespace Obbligato {
namespace Net {
const char *linkstatusmacosx_file = __FILE__;
}
}

#endif
