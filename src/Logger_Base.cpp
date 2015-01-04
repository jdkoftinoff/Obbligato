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
#include "Obbligato/Logger_Base.hpp"
#include "Obbligato/LoggerStream.hpp"
#include "Obbligato/Config.hpp"

namespace Obbligato
{

bool Logger::enable_error = true;
bool Logger::enable_warning = true;
bool Logger::enable_info = true;
bool Logger::enable_debug = false;
bool Logger::enable_trace = false;

void Logger::addOptions( ::Obbligato::Config::OptionGroups &options,
                         bool for_test )
{
    options.add( "log", "Logging options" )
        .add( "error",
              "1",
              "Enable error logging",
              Logger::enable_error )
        .add( "warning",
              "1",
              "Enable warning logging",
              Logger::enable_warning )
        .add( "info", "1", "Enable info logging", Logger::enable_info )
        .add( "debug",
              for_test ? "1" : "0",
              "Enable debug logging",
              Logger::enable_debug )
        .add( "trace",
              for_test ? "1" : "0",
              "Enable trace logging",
              Logger::enable_trace );
}
}
