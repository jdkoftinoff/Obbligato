#pragma once

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

#include <string>
#include <ostream>
#include <sstream>
#include "Obbligato/Logger.hpp"

namespace Obbligato
{

class Logger;
extern std::shared_ptr<Logger> logger;

class Logger
{
  private:
    void outputLine( std::ostream & ) const {}

    template <typename FirstArg, typename... RestArgs>
    void outputLine( std::ostream &o,
                     FirstArg &firstArg,
                     RestArgs &... restArgs ) const
    {
        o << firstArg;
        outputLine( o, restArgs... );
    }

    template <typename... Args>
    std::string generateLine( Args &&... args ) const
    {
        std::ostringstream ostr;
        outputLine( ostr, args... );
        return ostr.str();
    }

  public:
    static bool enable_error;
    static bool enable_warning;
    static bool enable_info;
    static bool enable_debug;
    static bool enable_trace;

    Logger( const Logger & ) = delete;
    Logger &operator=( const Logger & ) = delete;
    Logger() {}

    static void addOptions( ::Obbligato::Config::OptionGroups &options,
                            bool for_test = false );

    static void enableAll()
    {
        enable_info = true;
        enable_debug = true;
        enable_warning = true;
        enable_error = true;
        enable_trace = true;
    }

    static void disableAll()
    {
        enable_info = false;
        enable_debug = false;
        enable_warning = false;
        enable_error = false;
        enable_trace = false;
    }

    virtual void emitErrorLine( std::string const &s ) = 0;
    virtual void emitWarningLine( std::string const &s ) = 0;
    virtual void emitInfoLine( std::string const &s ) = 0;
    virtual void emitDebugLine( std::string const &s ) = 0;
    virtual void emitTraceLine( std::string const &s ) = 0;

    template <typename... Args>
    void logError( Args &&... args )
    {
        if ( enable_error )
        {
            emitErrorLine( generateLine( args... ) );
        }
    }

    template <typename... Args>
    void logWarning( Args &&... args )
    {
        if ( enable_warning )
        {
            emitWarningLine( generateLine( args... ) );
        }
    }

    template <typename... Args>
    void logInfo( Args &&... args )
    {
        if ( enable_info )
        {
            emitInfoLine( generateLine( args... ) );
        }
    }

    template <typename... Args>
    void logDebug( Args &&... args )
    {
        if ( enable_debug )
        {
            emitDebugLine( generateLine( args... ) );
        }
    }

    template <typename... Args>
    void logTrace( Args &&... args )
    {
        if ( enable_trace )
        {
            emitTraceLine( generateLine( args... ) );
        }
    }
};
}
