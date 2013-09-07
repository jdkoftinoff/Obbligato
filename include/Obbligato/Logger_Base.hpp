#pragma once
#ifndef Obbligato_Logger_Base_hpp
#define Obbligato_Logger_Base_hpp

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

#include "Obbligato/Noncopyable.hpp"
#include "Obbligato/Time.hpp"
#include "Obbligato/SharedPtr.hpp"

#define ob_cerror ::Obbligato::logger->cerror()
#define ob_cwarning ::Obbligato::logger->cwarning()
#define ob_cinfo ::Obbligato::logger->cinfo()
#define ob_cdebug                                                              \
    ::Obbligato::logger->cdebug() << __FILE__ << ":" << __LINE__ << ":"
#define ob_ctrace                                                              \
    ::Obbligato::logger->ctrace() << __FILE__ << ":" << __LINE__ << ":"

namespace Obbligato {
namespace Logger {

class LoggerBase;

class LoggerStreamBuf : public std::stringbuf {
  protected:
    LoggerBase *m_logger;

  public:
    LoggerStreamBuf(LoggerBase *l) : m_logger(l) {}

  protected:
    virtual int sync();

  private:
    virtual void send_to_logger(std::string const &s) = 0;
};

class LoggerStreamBufError : public LoggerStreamBuf {
  public:
    LoggerStreamBufError(LoggerBase *l) : LoggerStreamBuf(l) {}

  protected:

    virtual void send_to_logger(std::string const &s);
};

class LoggerStreamBufWarning : public LoggerStreamBuf {
  public:
    LoggerStreamBufWarning(LoggerBase *l) : LoggerStreamBuf(l) {}

  protected:

    virtual void send_to_logger(std::string const &s);
};

class LoggerStreamBufInfo : public LoggerStreamBuf {
  public:
    LoggerStreamBufInfo(LoggerBase *l) : LoggerStreamBuf(l) {}

  protected:

    virtual void send_to_logger(std::string const &s);
};

class LoggerStreamBufDebug : public LoggerStreamBuf {
  public:
    LoggerStreamBufDebug(LoggerBase *l) : LoggerStreamBuf(l) {}

  protected:

    virtual void send_to_logger(std::string const &s);
};

class LoggerStreamBufTrace : public LoggerStreamBuf {
  public:
    LoggerStreamBufTrace(LoggerBase *l) : LoggerStreamBuf(l) {}

  protected:

    virtual void send_to_logger(std::string const &s);
};

class LoggerBase : Noncopyable {
    LoggerStreamBufError m_cerror_buf;
    LoggerStreamBufWarning m_cwarning_buf;
    LoggerStreamBufInfo m_cinfo_buf;
    LoggerStreamBufDebug m_cdebug_buf;
    LoggerStreamBufTrace m_ctrace_buf;

    std::ostream m_cerror;
    std::ostream m_cwarning;
    std::ostream m_cinfo;
    std::ostream m_cdebug;
    std::ostream m_ctrace;

  public:
    LoggerBase()
        : m_cerror_buf(this), m_cwarning_buf(this), m_cinfo_buf(this),
          m_cdebug_buf(this), m_ctrace_buf(this), m_cerror(&m_cerror_buf),
          m_cwarning(&m_cwarning_buf), m_cinfo(&m_cinfo_buf),
          m_cdebug(&m_cdebug_buf), m_ctrace(&m_ctrace_buf) {}

    virtual ~LoggerBase() {}

    static bool enable_error;
    static bool enable_warning;
    static bool enable_info;
    static bool enable_debug;
    static bool enable_trace;

    void enable_all() {
        enable_info = true;
        enable_debug = true;
        enable_warning = true;
        enable_error = true;
        enable_trace = true;
    }

    virtual void error(std::string txt) = 0;
    virtual void warning(std::string txt) = 0;
    virtual void info(std::string txt) = 0;
    virtual void debug(std::string loc, std::string txt) = 0;
    virtual void trace(std::string loc, std::string txt) = 0;

    std::ostream &cerror() { return m_cerror; }
    std::ostream &cwarning() { return m_cwarning; }
    std::ostream &cinfo() { return m_cinfo; }
    std::ostream &cdebug() { return m_cdebug; }
    std::ostream &ctrace() { return m_ctrace; }
};
}
}

#endif
