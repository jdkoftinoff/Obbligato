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
#include "Obbligato/Net_Packet.hpp"
#include "Obbligato/IOStream.hpp"

namespace Obbligato
{
namespace Net
{

std::ostream &operator<<( std::ostream &o, Packet const &v )
{
    using namespace ::Obbligato::IOStream;

    o << label_fmt( "timestamp" );
    o << dec_fmt( v.timestamp() );
    o << std::endl;

    o << label_fmt( "network_port_address" );
    o << fmt( v.network_port_address() );
    o << std::endl;

    o << label_fmt( "source_address" );
    o << fmt( v.source_address() );
    o << std::endl;

    o << label_fmt( "destination_address" );
    o << fmt( v.destination_address() );
    o << std::endl;

    o << label_fmt( "protocol" );
    o << hex_fmt( v.protocol() );
    o << std::endl;

    PacketPayload const &vp = v.payload();

    o << vp;
    o << std::endl;

    return o;
}

std::istream &operator>>( std::istream &i, Packet &v )
{
    using namespace ::Obbligato::IOStream;

    v.clear();
    i >> label_unfmt( "timestamp" );
    i >> dec_unfmt( v.m_timestamp );

    i >> label_unfmt( "network_port_address" );
    i >> unfmt( v.m_network_port_address );

    i >> label_unfmt( "source_address" );
    i >> unfmt( v.m_source_address );

    i >> label_unfmt( "destination_address" );
    i >> unfmt( v.m_destination_address );

    i >> label_unfmt( "protocol" );
    i >> hex_unfmt( v.m_protocol );

    PacketPayload &vp = v.payload();
    i >> vp;

    return i;
}
}
}
