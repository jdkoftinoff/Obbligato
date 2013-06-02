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
#include "Obbligato/Tests_IOStream.hpp"
#include "Obbligato/IOStream.hpp"

namespace Obbligato { namespace Tests {

bool test_iostream()
{
    using namespace Obbligato::IOStream;

    ob_cinfo << title_fmt("ThisIsATitle") << std::endl;
    ob_cinfo << label_fmt("ThisIsALabel") << std::endl;
    ob_cinfo << fmt(true) << " " << fmt(false) << std::endl;
    ob_cinfo << fmt(uint16_t(0xaabb)) << std::endl;
    ob_cinfo << fmt(uint32_t(0xaabbccdd)) << std::endl;
    ob_cinfo << fmt(uint64_t(0xaabbccddeeff0011)) << std::endl;
    ob_cinfo << fmt( IEEE::MAC48(0x001cab000001)) << std::endl;
    ob_cinfo << fmt( IEEE::EUI64(0x001cabfffe000001)) << std::endl;
    ob_cinfo << bin_fmt(uint32_t(0xaabbccdd)) << std::endl;
    ob_cinfo << fmt_obj(uint32_t(0xaabbccdd),IOStream::BinFormat<uint32_t>()) << std::endl;
    ob_cinfo << fmt(std::string("This is a test with \"Quotes,\" \ttabs\tand newlines\n\n\n" ) ) << std::endl;

    return false;
}

}}


