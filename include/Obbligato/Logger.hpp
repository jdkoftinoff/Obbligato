#pragma once
#ifndef Obbligato_Logger_hpp
#define Obbligato_Logger_hpp

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

#include "Obbligato/Logger_Base.hpp"
#include "Obbligato/SharedPtr.hpp"
#include "Obbligato/Form.hpp"

#define ob_log_error(txt, ...)                                                 \
    do {                                                                       \
        if (::Obbligato::logger->enable_error) {                               \
            ::Obbligato::logger->error(                                        \
                ::Obbligato::form<4096>(txt, ##__VA_ARGS__));                  \
        }                                                                      \
    } while (false)
#define ob_log_warning(txt, ...)                                               \
    do {                                                                       \
        if (::Obbligato::logger->enable_warning) {                             \
            ::Obbligato::logger->warning(                                      \
                ::Obbligato::form<4096>(txt, ##__VA_ARGS__));                  \
        }                                                                      \
    } while (false)
#define ob_log_info(txt, ...)                                                  \
    do {                                                                       \
        if (::Obbligato::logger->enable_info) {                                \
            ::Obbligato::logger->info(                                         \
                ::Obbligato::form<4096>(txt, ##__VA_ARGS__));                  \
        }                                                                      \
    } while (false)
#define ob_log_debug(txt, ...)                                                 \
    do {                                                                       \
        if (::Obbligato::logger->enable_debug) {                               \
            ::Obbligato::logger->debug(                                        \
                ::Obbligato::form<4096>("%s:%d:", __FILE__, __LINE__),         \
                ::Obbligato::form<4096>(txt, ##__VA_ARGS__).c_str());          \
        }                                                                      \
    } while (false)
#define ob_log_trace(txt, ...)                                                 \
    do {                                                                       \
        if (::Obbligato::logger->enable_trace) {                               \
            ::Obbligato::logger->trace(                                        \
                ::Obbligato::form<4096>("%s:%d:", __FILE__, __LINE__),         \
                ::Obbligato::form<4096>(txt, ##__VA_ARGS__).c_str());          \
        }                                                                      \
    } while (false)

namespace Obbligato {
namespace Logger {

void logger_factory_add_options(::Obbligato::Config::OptionGroups &options,
                                bool for_test = false);
shared_ptr<LoggerBase> logger_factory_create_logger();
}
}

namespace Obbligato {
extern shared_ptr<Logger::LoggerBase> logger;
}

#endif
