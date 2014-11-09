#pragma once

/*
 Copyright (c) 2014, J.D. Koftinoff Software, Ltd. <jeffk@jdkoftinoff.com>
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

#include <string>
#include <ostream>
#include <sstream>

#if defined( __APPLE__ ) || defined( __linux__ )

#include <syslog.h>

#define OBBLIGATO_HAS_SYSLOG ( 1 )

#include "Obbligato/Logger_Base.hpp"

namespace Obbligato
{

class LoggerSyslog : public Logger
{
  public:
    LoggerSyslog();
    ~LoggerSyslog();

    static std::string syslog_identity;

    static void addOptions( ::Obbligato::Config::OptionGroups &options, bool for_test );

    virtual void emitErrorLine( std::string const &s ) { syslog( LOG_ERR, "%s", s.c_str() ); }
    virtual void emitWarningLine( std::string const &s ) { syslog( LOG_WARNING, "%s", s.c_str() ); }
    virtual void emitInfoLine( std::string const &s ) { syslog( LOG_INFO, "%s", s.c_str() ); }
    virtual void emitDebugLine( std::string const &s ) { syslog( LOG_DEBUG, "%s", s.c_str() ); }
    virtual void emitTraceLine( std::string const &s ) { syslog( LOG_DEBUG, "%s", s.c_str() ); }
};
}

#else

#define OBBLIGATO_HAS_SYSLOG ( 0 )

#endif
