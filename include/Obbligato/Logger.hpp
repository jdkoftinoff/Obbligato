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

#include "Obbligato/Logger_Base.hpp"
#include "Obbligato/LoggerNull.hpp"
#include "Obbligato/LoggerStream.hpp"
#include "Obbligato/LoggerSyslog.hpp"

#if !defined( ob_log_error )
#define ob_log_error( ... )                                            \
    do                                                                 \
    {                                                                  \
        if ( ::Obbligato::Logger::enable_error )                       \
        {                                                              \
            ::Obbligato::logger->logError( __VA_ARGS__ );              \
        }                                                              \
    } while ( false )
#endif

#if !defined( ob_log_warning )
#define ob_log_warning( ... )                                          \
    do                                                                 \
    {                                                                  \
        if ( ::Obbligato::Logger::enable_warning )                     \
        {                                                              \
            ::Obbligato::logWarning( __VA_ARGS__ );                    \
        }                                                              \
    } while ( false )
#endif

#if !defined( ob_log_info )
#define ob_log_info( ... )                                             \
    do                                                                 \
    {                                                                  \
        if ( ::Obbligato::Logger::enable_info )                        \
        {                                                              \
            ::Obbligato::logger->logInfo( __VA_ARGS__ );               \
        }                                                              \
    } while ( false )
#endif

#if !defined( ob_log_debug )
#define ob_log_debug( ... )                                            \
    do                                                                 \
    {                                                                  \
        if ( ::Obbligato::Logger::enable_debug )                       \
        {                                                              \
            ::Obbligato::logger->logDebug( __VA_ARGS__ );              \
        }                                                              \
    } while ( false )
#endif

#if !defined( ob_log_trace )
#define ob_log_trace( ... )                                            \
    do                                                                 \
    {                                                                  \
        if ( ::Obbligato::Logger::enable_trace )                       \
        {                                                              \
            ::Obbligato::logger->logTrace( __VA_ARGS__ );              \
        }                                                              \
    } while ( false )
#endif
