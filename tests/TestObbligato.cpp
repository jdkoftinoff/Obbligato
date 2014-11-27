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
#include "Obbligato.hpp"

#include "Obbligato/Test.hpp"

#include "Obbligato/Tests_Config.hpp"
#include "Obbligato/Tests_IOStream.hpp"
#include "Obbligato/Tests_LexicalCast.hpp"
#include "Obbligato/Tests_Logger.hpp"
#include "Obbligato/Tests_SIMD.hpp"
#include "Obbligato/Tests_Time.hpp"
#include "Obbligato/Tests_DSP.hpp"

int main( int, char const **argv )
{
    Obbligato::Test::Harness harness( argv );

    using namespace Obbligato::Tests;
    Obbligato::logger->enableAll();

    ob_log_info( "Starting TestObbligato" );

    OB_RUN_TEST( test_config, "Config" );
    OB_RUN_TEST( test_iostream, "IOStream" );
    OB_RUN_TEST( test_lexicalcast, "LexicalCast" );
    OB_RUN_TEST( test_logger, "Logger" );
    OB_RUN_TEST( test_simd, "SIMD" );
    OB_RUN_TEST( test_dsp, "DSP" );
    OB_RUN_TEST( test_time, "Time" );

    return harness.result_code();
}
