#pragma once

/*
 Copyright (c) 2014, J.D. Koftinoff Software, Ltd.
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

#include <string>
#include <ostream>
#include <sstream>
#include "Obbligato/Logger_Base.hpp"

namespace Obbligato
{

class LoggerNull : public Logger
{
  public:
    LoggerNull() {}

    static void addOptions( ::Obbligato::Config::OptionGroups &options,
                            bool for_test )
    {
    }

    virtual void emitErrorLine( std::string const & ) {}
    virtual void emitWarningLine( std::string const & ) {}
    virtual void emitInfoLine( std::string const & ) {}
    virtual void emitDebugLine( std::string const & ) {}
    virtual void emitTraceLine( std::string const & ) {}
};
}
