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
        std::ostream &operator << ( std::ostream &o, Packet const &v )
        {
            using namespace ::Obbligato::IOStream;

            o << fmt(label("timestamp")) << fmt(v.timestamp()) << std::endl;
            o << fmt(label("network_port_address")) << fmt(v.network_port_address() ) << std::endl;
            o << fmt(label("source_address")) << fmt(v.source_address() ) << std::endl;
            o << fmt(label("destination_address")) << fmt(v.destination_address() ) << std::endl;
            o << fmt(label("protocol")) << fmt(v.protocol()) << std::endl;

            PacketPayload const &vp = v;

            o << vp;
            return o;
        }

        template <typename T>
        inline typename IOStream::DefaultFormat<T>::unformatter_type duf(T &v)
        {
            return typename IOStream::DefaultFormat<T>::unformatter_type(v);
        }

        std::istream &operator >> ( std::istream &i, Packet &v )
        {
            using namespace ::Obbligato::IOStream;

            i >> unfmt(label("timestamp") );
            i >> unfmt(v.m_timestamp);

            i >> unfmt(label("network_port_address"));
            i >> unfmt(v.m_network_port_address);

            i >> unfmt(label("source_address"));
            i >> unfmt(v.m_source_address);

            i >> unfmt(label("destination_address"));
            i >> unfmt(v.m_destination_address);

            i >> unfmt(label("protocol"));
            i >> unfmt(v.m_protocol);

            PacketPayload &vp = v;
            i >> vp;

            return i;
        }

    }
}

