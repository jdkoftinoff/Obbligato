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

class LoggerStream : public Logger
{
  public:
    std::ostream *m_cerror;
    std::ostream *m_cwarning;
    std::ostream *m_cinfo;
    std::ostream *m_cdebug;
    std::ostream *m_ctrace;

    LoggerStream();

    static void addOptions( ::Obbligato::Config::OptionGroups &options,
                            bool for_test );

    virtual void emitErrorLine( std::string const &s )
    {
        ( *m_cerror ) << "ERROR   : " << s << std::endl;
    }
    virtual void emitWarningLine( std::string const &s )
    {
        ( *m_cwarning ) << "WARNING : " << s << std::endl;
    }
    virtual void emitInfoLine( std::string const &s )
    {
        ( *m_cinfo ) << "INFO    : " << s << std::endl;
    }
    virtual void emitDebugLine( std::string const &s )
    {
        ( *m_cdebug ) << "DEBUG   : " << s << std::endl;
    }
    virtual void emitTraceLine( std::string const &s )
    {
        ( *m_ctrace ) << "TRACE   : " << s << std::endl;
    }
};
}
