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
#include "Obbligato/Tests_Net.hpp"
#include "Obbligato/Net.hpp"

namespace Obbligato {
namespace Tests {

using namespace Obbligato;
using namespace IOStream;
using namespace Obbligato::Net;

bool parse_ipv4() {
    Address a;
    std::string from = "192.168.0.1";
    std::string to;

    a.from_string(from, AF_INET);
    to = a.to_string();

    ob_cinfo << "Hello" << std::endl;

    ob_cinfo << "from:" << from << " To " << to << std::endl;

    return from == to;
}

bool parse_ipv4_auto() {
    Address a;
    std::string from = "192.168.0.1";
    std::string to;

    a.from_string(from);
    to = a.to_string();

    ob_cinfo << "from:" << from << " To " << to << std::endl;

    return from == to;
}

bool parse_ipv6() {
    Address a;
    std::string from = "fe80::1234:5678";
    std::string to;

    a.from_string(from, AF_INET6);
    to = a.to_string();

    ob_cinfo << "from:" << from << " To " << to << std::endl;

    return from == to;
}

bool parse_ipv6_auto() {
    Address a;
    std::string from = "fe80::1234:5678";
    std::string to;

    a.from_string(from);
    to = a.to_string();

    ob_cinfo << "from:" << from << " To " << to << std::endl;

    return from == to;
}

bool parse_mac48() {
    Address a;
    std::string from = "00-1C-AB-00-00-01";
    std::string to;

#if defined(AF_LINK)
    a.from_string(from, AF_LINK);
#elif defined(AF_PACKET)
    a.from_string(from, AF_PACKET);
#else
#error please define platform specific mac48 sockaddr family
#endif
    to = a.to_string();

    ob_cinfo << "from:" << from << " To " << to << std::endl;

    return from == to;
}

bool parse_mac48_auto() {
    Address a;
    std::string from = "00-1C-AB-00-00-01";
    std::string to;

    a.from_string(from);
    to = a.to_string();

    ob_cinfo << "from:" << from << " To " << to << std::endl;

    return from == to;
}

bool parse_mac48_stream() {
    std::string from = "00-1C-AB-00-00-01";
    std::stringstream str;
    str << from;

    MAC48 b;
    str >> eui_unfmt(b);
    std::string to = b.to_string();

    ob_cinfo << "from:" << from << " To " << to << std::endl;

    return from == to;
}

bool parse_ipv4_stream() {
    std::string from = "192.168.0.1";
    std::stringstream str;
    str << from;

    Address b;
    str >> unfmt(b);
    std::string to = b.to_string();

    ob_cinfo << "from:" << from << " To " << to << std::endl;

    return from == to;
}

bool parse_ipv6_stream() {
    std::string from = "fe80::baf6:b1ff:fe15:dcd5";
    std::stringstream str;
    str << from;

    Address b;
    str >> unfmt(b);
    std::string to = b.to_string();

    ob_cinfo << "from:" << from << " To " << to << std::endl;

    return from == to;
}

bool test_Address() {
    bool r = true;
    std::string group = "Address";
    r &= OB_RUN_TEST(parse_ipv4, group);
    r &= OB_RUN_TEST(parse_ipv4_auto, group);
    r &= OB_RUN_TEST(parse_ipv6, group);
    r &= OB_RUN_TEST(parse_ipv6_auto, group);
    r &= OB_RUN_TEST(parse_mac48, group);
    r &= OB_RUN_TEST(parse_mac48_auto, group);
    r &= OB_RUN_TEST(parse_mac48_stream, group);
    r &= OB_RUN_TEST(parse_ipv4_stream, group);
    r &= OB_RUN_TEST(parse_ipv6_stream, group);
    return r;
}

bool lookup_Address() {
    AddressList a = make_addresslist(GetAddrInfoForTcp("", "http", true, true));

    std::stringstream str;

    str << a;

    ob_cinfo << "Result of lookup: " << str.str() << std::endl;

    AddressList b;
    str >> b;

    for (AddressList::const_iterator i = b.begin(); i != b.end(); ++i) {
        ob_cinfo << "Retrieved from string: " << i->to_string() << std::endl;
    }

    return true;
}

bool test_Addresslist() {
    bool r = true;
    std::string group = "AddressList";
    r &= OB_RUN_TEST(lookup_Address, group);
    return r;
}

bool packetpayload_iostream() {
    PacketPayload payload;

    for (int i = 0; i < 150; ++i) {
        payload.data.push_back(static_cast<uint8_t>(i));
    }

    std::stringstream str;
    str << payload;
    ob_cinfo << title_fmt("str contains") << std::endl << str.str() << std::endl;

    PacketPayload result;
    str >> result;

    std::stringstream str1;
    str1 << result;

    ob_cinfo << title_fmt("result contains") << std::endl << str1.str() << std::endl;

    return str1.str() == str.str();
}

bool test_netpacketpayload() {
    bool r = true;
    std::string group = "PacketPayload";
    r &= OB_RUN_TEST(packetpayload_iostream, group);
    return r;
}

bool packet_iostream() {
    Packet pkt;

    pkt.set_timestamp(Time::get_current_timestamp());
    for (int i = 0; i < 150; ++i) {
        pkt.payload().data.push_back(static_cast<uint8_t>(i));
    }
    pkt.set_source_address(Address("192.168.0.1"));
    pkt.set_destination_address(Address("192.168.0.2"));
    pkt.set_network_port_address(Address("192.168.0.1"));
    pkt.set_protocol(80);

    std::stringstream str;
    str << pkt;
    ob_cinfo << title_fmt("str contains") << std::endl;
    ob_cinfo << str.str() << std::endl;

    Packet result;
    str >> result;

    std::stringstream str1;
    str1 << result;

    ob_cinfo << title_fmt("str1 contains") << std::endl;
    ob_cinfo << str1.str() << std::endl;
    
    return str1.str() == str.str();
}

bool test_netpacket() {
    bool r = true;
    std::string group = "Packet";
    r &= OB_RUN_TEST(packet_iostream, group);
    return r;
}

bool test_net() {
    bool r = true;
    r &= test_Address();
    r &= test_Addresslist();
    r &= test_netpacketpayload();
    r &= test_netpacket();
    return r;
}
}
}
