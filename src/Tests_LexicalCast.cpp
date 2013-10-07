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
#include "Obbligato/Tests_LexicalCast.hpp"

namespace Obbligato {
namespace Tests {

bool test_lexicalcast() {
    using namespace Obbligato::IOStream;
    std::string s;
    bool y = false;
    s = to_string(y);

    bool x = from_string<bool>(s);

    ob_cinfo << bool_fmt(x) << " " << bool_fmt(y) << std::endl;

    std::string f = "Test\tOf\nEscaped\tString";
    s = to_string(f);

    std::string t = from_string<std::string>(s);

    std::stringstream ss(s);
    std::string u;
    ss >> IOStream::stringblock_unfmt(u);

    ob_cinfo << s << " " << stringblock_fmt(t) << " " << stringblock_fmt(u)
             << std::endl;

    return x == y && f == t;
}
}
}
