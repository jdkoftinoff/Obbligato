/*
 Copyright (c) 2013, J.D. Koftinoff Software, Ltd.
 <jeffk@jdkoftinoff.com>
 http://www.jdkoftinoff.com/
 All rights reserved.

 Permission to use, copy, modify, and/or distribute this software for
 any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.

 THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 WARRANTIES
 WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "Obbligato/World.hpp"
#include "Obbligato/Test.hpp"

namespace Obbligato
{
namespace Test
{

Config::OptionGroups Harness::options;
Harness *harness = 0;

Harness::Harness( char const **argv )
    : m_test_count( 0 )
    , m_fail_count( 0 )
    , m_success_count( 0 )
    , m_exception_count( 0 )
{
    static std::string logger_type = "stream";

    harness = this;
    Logger::addOptions( options, true );
    LoggerStream::addOptions( options, true );

#if OBBLIGATO_HAS_SYSLOG
    LoggerSyslog::addOptions( options, true );
    options.add( "logger", "Logger" )
        .add( "type",
              "stream",
              "Type of Logger (null,stream,syslog)",
              logger_type );
#endif

    if ( !options.parse( argv, __DATE__ ) )
    {
        exit( 1 );
    }

    if ( logger_type == "null" )
    {
        logger = std::unique_ptr<Logger>( new LoggerNull );
    }
    else if ( logger_type == "stream" )
    {
        logger = std::unique_ptr<Logger>( new LoggerStream );
    }
#if OBBLIGATO_HAS_SYSLOG
    else if ( logger_type == "syslog" )
    {
        logger = std::unique_ptr<Logger>( new LoggerSyslog );
    }
    std::cin.exceptions( std::istream::failbit | std::istream::badbit
                         | std::istream::eofbit );
#endif

    ob_log_info( "Unit Test for ", argv[0], " ", __DATE__ );
}

Harness::~Harness()
{
    using namespace IOStream;
    ob_log_info( label_fmt( "Total Tests" ), m_test_count );
    ob_log_info( label_fmt( "Total Fail" ), m_fail_count );
    ob_log_info( label_fmt( "Total Pass" ), m_success_count );
    ob_log_info( label_fmt( "Total Exceptions" ), m_exception_count );
    ob_log_info( label_fmt( "Result" ),
                 ( m_fail_count == 0 ? "PASS" : "FAIL" ) );
}

bool Harness::result() const
{
    return m_fail_count == 0;
    ;
}

int Harness::result_code() const { return m_fail_count == 0 ? 0 : 1; }
}
}
