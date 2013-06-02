#pragma once
#ifndef Obbligato_Test_hpp
#define Obbligato_Test_hpp

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

#define OB_RUN_TEST(func,group) ::Obbligato::Test::harness->run_test( func, group, std::string(#func) )

namespace Obbligato { namespace Test {

class Harness;
extern Harness *harness;

class Harness
{
    static Config::OptionGroups options;
    Atomic::AtomicInt m_test_count;
    Atomic::AtomicInt m_fail_count;
    Atomic::AtomicInt m_success_count;
    Atomic::AtomicInt m_exception_count;

public:
    Harness( char const **argv )
        : m_test_count(0),
          m_fail_count(0),
          m_success_count(0),
          m_exception_count(0)
    {
        harness = this;
        Logger::logger_factory_add_options(options,true);
        if( !options.parse(argv, __DATE__ ) )
        {
            exit(1);
        }
        std::cin.exceptions( std::istream::failbit | std::istream::badbit | std::istream::eofbit );
        Logger::logger_factory_create_logger();
        ob_cinfo << "Unit Test for " << argv[0] << " " << __DATE__ << std::endl;
    }

    ~Harness()
    {
        using namespace IOStream;
        ob_cinfo << label_fmt("Total Tests") << m_test_count << std::endl;
        ob_cinfo << label_fmt("Total Fail") << m_fail_count << std::endl;
        ob_cinfo << label_fmt("Total Pass") << m_success_count << std::endl;
        ob_cinfo << label_fmt("Total Exceptions") << m_exception_count << std::endl;
        ob_cinfo << label_fmt("Result") << (m_fail_count==0 ? "PASS" : "FAIL" ) << std::endl;
    }

    bool result() const
    {
        return m_fail_count == 0;;
    }

    int result_code() const
    {
        return m_fail_count==0 ? 0 : 1;
    }

    template <typename Func>
    inline bool run_test( Func f, std::string group, std::string testname = std::string("") )
    {
        using namespace IOStream;
        bool r=false;
        m_test_count++;
        try
        {
            ob_cinfo << std::endl;
            ob_cinfo << title_fmt("TEST") << group << ":" << testname << std::endl;
            r=f();
            if( r )
            {
                ob_cinfo << title_fmt("SUCCESS") << group << ":" << testname << std::endl;
                m_success_count++;
            }
            else
            {
                ob_cinfo << title_fmt("FAIL") << group << ":" << testname << std::endl;
                m_fail_count++;
            }
        }
        catch (std::out_of_range const &e )
        {
            ob_cinfo << title_fmt("FAIL") << group << ":" << testname << std::endl;
            m_fail_count++;
            ob_cinfo << title_fmt("EXCEPTION") << group << ":" << testname << ":" << "out_of_range(\"" << e.what() << "\")" << std::endl;
            m_exception_count++;
            return false;
        }
        catch (std::invalid_argument const &e )
        {
            ob_cinfo << title_fmt("FAIL") << group << ":" << testname << std::endl;
            m_fail_count++;
            ob_cinfo << title_fmt("EXCEPTION") << group << ":" << testname << ":" << "invalid_argument(\"" << e.what() << "\")" << std::endl;
            m_exception_count++;
            return false;
        }
        catch (std::range_error const &e )
        {
            ob_cinfo << title_fmt("FAIL") << group << ":" << testname << std::endl;
            m_fail_count++;
            ob_cinfo << title_fmt("EXCEPTION") << group << ":" << testname << ":" << "range_error(\"" << e.what() << "\")" << std::endl;
            m_exception_count++;
            return false;
        }
        catch (std::runtime_error const &e)
        {
            ob_cinfo << title_fmt("FAIL") << group << ":" << testname << std::endl;
            m_fail_count++;
            ob_cinfo << title_fmt("EXCEPTION") << group << ":" << testname << ":" << "runtime_error(\"" << e.what() << "\")" << std::endl;
            m_exception_count++;
            return false;
        }
        catch (std::logic_error const &e )
        {
            ob_cinfo << title_fmt("FAIL") << group << ":" << testname << std::endl;
            m_fail_count++;
            ob_cinfo << title_fmt("EXCEPTION") << group << ":" << testname << ":" << "logic_error(\"" << e.what() << "\")" << std::endl;
            m_exception_count++;
            return false;
        }
        catch (std::exception const &e )
        {
            ob_cinfo << title_fmt("FAIL") << group << ":" << testname << std::endl;
            m_fail_count++;
            ob_cinfo << title_fmt("EXCEPTION") << group << ":" << testname << ":" << "exception(\"" << e.what() << "\")" << std::endl;
            m_exception_count++;
            return false;
        }

        return r;
    }


};



}}

#endif


