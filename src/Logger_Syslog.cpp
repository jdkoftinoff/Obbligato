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
#include "Obbligato/Logger_Syslog.hpp"

#if defined(WIN32) 
namespace Obbligato {
namespace Logger {
static const char *syslog_file = __FILE__;
}}

#elif defined(_POSIX_VERSION)

namespace Obbligato {
namespace Logger {

LoggerSyslog::LoggerSyslog(std::string ident, int logopt, int facility) {
    ::openlog(ident.c_str(), logopt, facility);
}

LoggerSyslog::~LoggerSyslog() { ::closelog(); }

void LoggerSyslog::error(std::string txt) {
    syslog(LOG_ERR, "%s", txt.c_str());
}

void LoggerSyslog::warning(std::string txt) {
    syslog(LOG_WARNING, "%s", txt.c_str());
}

void LoggerSyslog::info(std::string txt) {
    syslog(LOG_INFO, "%s", txt.c_str());
}

void LoggerSyslog::debug(std::string loc, std::string txt) {
    syslog(LOG_DEBUG, "%s%s", loc.c_str(), txt.c_str());
}

void LoggerSyslog::trace(std::string loc, std::string txt) {
    syslog(LOG_DEBUG, "%s%s", loc.c_str(), txt.c_str());
}
}
}
#endif
