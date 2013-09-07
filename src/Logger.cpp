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
shared_ptr<Logger::LoggerBase> logger =
    make_shared<Logger::LoggerIOStream>(std::cout, std::cerr);
}

namespace Obbligato {
namespace Logger {

bool LoggerBase::enable_error;
bool LoggerBase::enable_warning;
bool LoggerBase::enable_info;
bool LoggerBase::enable_debug;
bool LoggerBase::enable_trace;

std::string logger_factory_type = "stdio";
std::string logger_factory_destination = "log.txt";

void logger_factory_add_options(::Obbligato::Config::OptionGroups &options,
                                bool for_test) {
    options.add("log", "Logging options")
        .add("type", "stdio", "Logger type (stdio,syslog,file,win32)",
             logger_factory_type)
        .add("destination", "log.txt", "Logger destination",
             logger_factory_destination)
        .add("error", "true", "Enable error logging", logger->enable_error)
        .add("warning", "true", "Enable warning logging",
             logger->enable_warning)
        .add("info", "true", "Enable info logging", logger->enable_info)
        .add("debug", for_test ? "true" : "false", "Enable debug logging",
             logger->enable_debug)
        .add("trace", for_test ? "true" : "false", "Enable trace logging",
             logger->enable_trace);
}

shared_ptr<LoggerBase> logger_factory_create_logger() {
    shared_ptr<LoggerBase> l;

#ifndef WIN32
    if (logger_factory_type == "syslog") {
        l = make_shared<LoggerSyslog>();
        if (l) {
            logger = l;
        }
        return logger;
    }
#endif
    if (logger_factory_type == "file") {
        l = make_shared<LoggerFile>(logger_factory_destination);
        if (l) {
            logger = l;
        }
        return logger;
    }
#if 0 && defined(WIN32)
    if( logger_factory_type=="win32")
    {
        l = make_shared<LoggerWin32>( logger_factory_destination );
        if( l )
        {
            logger=l;
        }
        return logger;
    }
#endif
    return logger;
}
}
}
