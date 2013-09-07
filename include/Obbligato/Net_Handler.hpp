#pragma once
#ifndef Obbligato_Net_Handler_hpp
#define Obbligato_Net_Handler_hpp

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
#include "Obbligato/SharedPtr.hpp"
#include "Obbligato/Time.hpp"

namespace Obbligato {
namespace Net {

class Handler;
typedef shared_ptr<Handler> HandlerPtr;
typedef std::map<SOCKET, HandlerPtr> HandlerPtrMap;

class Handler : public Time::Ticker {
  public:
    virtual ~Handler() {}

    /// Returns true if the object is ready for business
    virtual bool is_open() const = 0;

    /// Returns true if the object is interested in being notified when the file
    /// handle is readable
    virtual bool wake_on_readable() const = 0;

    /// Returns true if the object is interested in being notified when the file
    /// handle is writable
    virtual bool wake_on_writable() const = 0;

    /// Returns the file handle
    virtual SOCKET fd() const = 0;

    /// Notification that the file handle was closed
    virtual void closed() = 0;

    /// Notification that the socket encountered some sort of error
    virtual void error() = 0;

    /// Notification that the file handle is readable. Returns false to trigger
    /// an end to the dispatcher.
    virtual bool readable() = 0;

    /// Notification that the file handle is writable. Returns false to trigger
    /// an end to the dispatcher.
    virtual bool writable() = 0;

    /// Notification some time has passed.
    virtual void ticker_tick(Timestamp timestamp) = 0;
};
}
}

#endif
