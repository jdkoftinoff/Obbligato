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
#include "Obbligato/Test.hpp"

namespace Obbligato {
namespace Test {

Config::OptionGroups Harness::options;
Harness *harness = 0;

Harness::Harness(char const **argv)
    : m_test_count(0), m_fail_count(0), m_success_count(0),
      m_exception_count(0) {
    harness = this;
    Logger::logger_factory_add_options(options, true);
    if (!options.parse(argv, __DATE__)) {
        exit(1);
    }
    std::cin.exceptions(std::istream::failbit | std::istream::badbit |
                        std::istream::eofbit);
    Logger::logger_factory_create_logger();
    ob_cinfo << "Unit Test for " << argv[0] << " " << __DATE__ << std::endl;
}

Harness::~Harness() {
    using namespace IOStream;
    ob_cinfo << label_fmt("Total Tests") << m_test_count << std::endl;
    ob_cinfo << label_fmt("Total Fail") << m_fail_count << std::endl;
    ob_cinfo << label_fmt("Total Pass") << m_success_count << std::endl;
    ob_cinfo << label_fmt("Total Exceptions") << m_exception_count << std::endl;
    ob_cinfo << label_fmt("Result") << (m_fail_count == 0 ? "PASS" : "FAIL")
             << std::endl;
}

bool Harness::result() const {
    return m_fail_count == 0;
    ;
}

int Harness::result_code() const { return m_fail_count == 0 ? 0 : 1; }
}
}
