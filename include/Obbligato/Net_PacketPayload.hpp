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
#include "Obbligato/IOStream.hpp"
#include "Obbligato/StaticAssert.hpp"
#include <array>

namespace Obbligato {
namespace Net {

struct PacketPayload {

    std::vector<uint8_t> data;
    
    PacketPayload()
        : data(1500)
    {
        data.clear();
    }
    
    PacketPayload( PacketPayload const &other )
        : data(other.data) {
    }
    
    PacketPayload( PacketPayload &&other )
        : data( std::move( other.data )) {
    }
    
    PacketPayload const & operator = ( PacketPayload const &other ) {
        data = other.data;
        return *this;
    }
    
    PacketPayload const & operator = ( PacketPayload &&other ) {
        data = std::move(other.data );
        return *this;
    }
    
};

std::ostream &operator<<(std::ostream &o, PacketPayload const &v);

std::istream &operator>>(std::istream &o, PacketPayload &v);


}
}

