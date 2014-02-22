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
#include "Obbligato/Logger.hpp"
#include "Obbligato/IOStream.hpp"
#include "Obbligato/Config.hpp"
#include "Obbligato/Atomic.hpp"

#define OB_RUN_TEST(func, group)                                               \
    ::Obbligato::Test::harness->run_test(func, group, std::string(#func))

namespace Obbligato {
namespace Test {

class Harness;
extern Harness *harness;

class Harness {
    static Config::OptionGroups options;
    std::atomic<int> m_test_count;
    std::atomic<int> m_fail_count;
    std::atomic<int> m_success_count;
    std::atomic<int> m_exception_count;

  public:
    Harness(char const **argv);

    ~Harness();

    bool result() const;

    int result_code() const;

    template <typename Func>
    inline bool run_test(Func f, std::string group,
                         std::string testname = std::string("")) {
        using namespace IOStream;
        bool r = false;
        m_test_count++;
        try {
            ob_log_info( title_fmt("TEST") << group << ":" << testname );
            r = f();
            if (r) {
                ob_log_info( title_fmt("SUCCESS") << group << ":" << testname);
                m_success_count++;
            } else {
                ob_log_info( title_fmt("FAIL") << group << ":" << testname);
                m_fail_count++;
            }
        }
        catch (std::out_of_range const &e) {
            ob_log_info( title_fmt("FAIL") << group << ":" << testname);
            m_fail_count++;
            ob_log_info( title_fmt("EXCEPTION") << group << ":" << testname
                     << ":"
                     << "out_of_range(\"" << e.what() << "\")" );
            m_exception_count++;
            return false;
        }
        catch (std::invalid_argument const &e) {
            ob_log_info( title_fmt("FAIL") << group << ":" << testname);
            m_fail_count++;
            ob_log_info( title_fmt("EXCEPTION") << group << ":" << testname
                     << ":"
                     << "invalid_argument(\"" << e.what() << "\")");
            m_exception_count++;
            return false;
        }
        catch (std::range_error const &e) {
            ob_log_info( title_fmt("FAIL") << group << ":" << testname);
            m_fail_count++;
            ob_log_info( title_fmt("EXCEPTION") << group << ":" << testname
                     << ":"
                     << "range_error(\"" << e.what() << "\")");
            m_exception_count++;
            return false;
        }
        catch (std::runtime_error const &e) {
            ob_log_info( title_fmt("FAIL") << group << ":" << testname);
            m_fail_count++;
            ob_log_info( title_fmt("EXCEPTION") << group << ":" << testname
                     << ":"
                     << "runtime_error(\"" << e.what() << "\")");
            m_exception_count++;
            return false;
        }
        catch (std::logic_error const &e) {
            ob_log_info( title_fmt("FAIL") << group << ":" << testname);
            m_fail_count++;
            ob_log_info( title_fmt("EXCEPTION") << group << ":" << testname
                     << ":"
                     << "logic_error(\"" << e.what() << "\")" );
            m_exception_count++;
            return false;
        }
        catch (std::exception const &e) {
            ob_log_info( title_fmt("FAIL") << group << ":" << testname );
            m_fail_count++;
            ob_log_info( title_fmt("EXCEPTION") << group << ":" << testname
                     << ":"
                     << "exception(\"" << e.what() << "\")" );
            m_exception_count++;
            return false;
        }

        return r;
    }
};
}
}
