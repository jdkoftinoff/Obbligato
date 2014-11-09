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
#include "Obbligato/Logger_Base.hpp"
#include "Obbligato/Config.hpp"

namespace Obbligato
{
namespace Logger
{

bool LoggerBase::enable_error = true;
std::ostream *LoggerBase::cerror = &std::clog;
bool LoggerBase::enable_warning = true;
std::ostream *LoggerBase::cwarning = &std::clog;
bool LoggerBase::enable_info = true;
std::ostream *LoggerBase::cinfo = &std::clog;
bool LoggerBase::enable_debug = false;
std::ostream *LoggerBase::cdebug = &std::clog;
bool LoggerBase::enable_trace = false;
std::ostream *LoggerBase::ctrace = &std::clog;

void LoggerBase::addOptions( ::Obbligato::Config::OptionGroups &options, bool for_test )
{
    options.add( "log", "Logging options" )
        .add( "error", "true", "Enable error logging", LoggerBase::enable_error )
        .add( "warning", "true", "Enable warning logging", LoggerBase::enable_warning )
        .add( "info", "true", "Enable info logging", LoggerBase::enable_info )
        .add( "debug", for_test ? "true" : "false", "Enable debug logging", LoggerBase::enable_debug )
        .add( "trace", for_test ? "true" : "false", "Enable trace logging", LoggerBase::enable_trace );
}
}
}
