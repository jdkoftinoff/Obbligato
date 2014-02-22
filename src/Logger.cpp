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
#include "Obbligato/Config_OptionGroups.hpp"
#include "Obbligato/Logger.hpp"
#include "Obbligato/Logger_IOStream.hpp"
#include "Obbligato/Logger_File.hpp"
#include "Obbligato/Logger_Syslog.hpp"

namespace Obbligato {
namespace Logger {
LoggerBase first_logger;
}
Logger::LoggerBase *logger=&Logger::first_logger;
}

namespace Obbligato {
namespace Logger {

bool enable_error=true;
std::ostream *cerror=&std::clog;
bool enable_warning=true;
std::ostream *cwarning=&std::clog;
bool enable_info=true;
std::ostream *cinfo=&std::clog;
bool enable_debug=false;
std::ostream *cdebug=&std::clog;
bool enable_trace=false;
std::ostream *ctrace=&std::clog;

bool LoggerBase::enable_error = true;
bool LoggerBase::enable_warning = false;
bool LoggerBase::enable_info = true;
bool LoggerBase::enable_debug = false;
bool LoggerBase::enable_trace = false;

void LoggerBase::add_options(::Obbligato::Config::OptionGroups &options,
                                bool for_test) {
    options.add("log", "Logging options")
        .add("error", "true", "Enable error logging", LoggerBase::enable_error)
        .add("warning", "true", "Enable warning logging",LoggerBase::enable_warning)
        .add("info", "true", "Enable info logging", LoggerBase::enable_info)
        .add("debug", for_test ? "true" : "false", "Enable debug logging",LoggerBase::enable_debug)
        .add("trace", for_test ? "true" : "false", "Enable trace logging",LoggerBase::enable_trace);
}

}
}
