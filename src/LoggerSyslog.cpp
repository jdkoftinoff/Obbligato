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
#include "Obbligato/LoggerSyslog.hpp"
#include "Obbligato/Config.hpp"

namespace Obbligato
{

std::string LoggerSyslog::syslog_identity = "daemon";

LoggerSyslog::LoggerSyslog()
{
    openlog(
        syslog_identity.c_str(), LOG_PID | LOG_NDELAY, LOG_DAEMON );
}

LoggerSyslog::~LoggerSyslog() { closelog(); }

void LoggerSyslog::addOptions(
    ::Obbligato::Config::OptionGroups &options, bool for_test )
{
    options.add( "syslog", "Logging options for syslog" )
        .add( "syslog_identity",
              "daemon",
              "syslog identity",
              LoggerSyslog::syslog_identity );
}
}
