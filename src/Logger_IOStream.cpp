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
#include "Obbligato/Logger_IOStream.hpp"

namespace Obbligato {
namespace Logger {

void LoggerIOStream::error(std::string txt) {
    m_err << "ERROR   @(" << Time::get_current_timestamp() << ")\t:" << txt
          << std::endl;
}

void LoggerIOStream::warning(std::string txt) {
    m_err << "WARNING @(" << Time::get_current_timestamp() << ")\t:" << txt
          << std::endl;
}

void LoggerIOStream::info(std::string txt) {
    m_out << "INFO    @(" << Time::get_current_timestamp() << ")\t:" << txt
          << std::endl;
}

void LoggerIOStream::debug(std::string loc, std::string txt) {
    m_out << "DEBUG   @(" << Time::get_current_timestamp() << ")\t:" << loc
          << txt << std::endl;
}

void LoggerIOStream::trace(std::string loc, std::string txt) {
    m_out << "TRACE   @(" << Time::get_current_timestamp() << ")\t:" << loc
          << txt << std::endl;
}
}
}
