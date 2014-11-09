#pragma once

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

#include "Obbligato/Time.hpp"
#include "Obbligato/SharedPtr.hpp"

namespace Obbligato
{
namespace Logger
{

class LoggerBase;

class LoggerBase
{
  public:
    static bool enable_error;
    static std::ostream *cerror;
    static bool enable_warning;
    static std::ostream *cwarning;
    static bool enable_info;
    static std::ostream *cinfo;
    static bool enable_debug;
    static std::ostream *cdebug;
    static bool enable_trace;
    static std::ostream *ctrace;

    LoggerBase( const LoggerBase & ) = delete;
    LoggerBase & operator = (const LoggerBase & ) = delete;
    LoggerBase() {}
    
    static void addOptions( ::Obbligato::Config::OptionGroups &options, bool for_test = false );

    static void enableAll()
    {
        enable_info = true;
        enable_debug = true;
        enable_warning = true;
        enable_error = true;
        enable_trace = true;
    }
};
}

extern Logger::LoggerBase *logger;
}
