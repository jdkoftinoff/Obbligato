#pragma once
#ifndef Obbligato_Logger_Syslog_hpp
#define Obbligato_Logger_Syslog_hpp

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

#if defined(_POSIX_VERSION)

#include <syslog.h>

namespace Obbligato { namespace Logger {

class LoggerSyslog : public LoggerBase
{
public:
    LoggerSyslog( std::string ident = "", int logopt = LOG_PID | LOG_CONS, int facility = LOG_DAEMON );
    ~LoggerSyslog();

    virtual void error( std::string txt );
    virtual void warning( std::string txt );
    virtual void info( std::string txt );
    virtual void debug( std::string loc, std::string txt );
    virtual void trace( std::string loc, std::string txt );

private:
};

}}
#endif

#endif


