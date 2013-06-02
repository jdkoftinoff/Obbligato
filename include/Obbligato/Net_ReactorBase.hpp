#pragma once
#ifndef Obbligato_Net_ReactorBase_hpp
#define Obbligato_Net_ReactorBase_hpp

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
#include "Obbligato/Net_Address.hpp"
#include "Obbligato/Net_Handler.hpp"

namespace Obbligato { namespace Net {

class NetReactorBase;
typedef shared_ptr<NetReactorBase> NetReactorBasePtr;

class NetReactorBase
{
public:
    NetReactorBase()
    {
    }

    virtual ~NetReactorBase()
    {
    }

    /// Track the specified Handler
    virtual void add( Handler *handler ) = 0;

    /// Stop tracking the specified Handler
    virtual void remove( Handler *handler ) = 0;

    /// Prepare for the poll() call
    virtual void prepare() = 0;

    /// Do the poll() and return within timeout_ms milliseconds.
    virtual bool poll(uint32_t timeout_ms) = 0;

    /// Dispatch the events to the Handlers
    virtual void dispatch() = 0;

    /// Notification some time has passed.
    virtual void tick( Timestamp timestamp ) = 0;

    /// Clean up, destroy, and remove any closed NetDispatchers.
    virtual void cleanup() = 0;

    /// Run transactions until there are no Handlers left
    virtual bool run( uint32_t timeout_ms ) = 0;

    /// Close all Handlers, forcing a orderly exit
    virtual void close_all() = 0;

};

}}

#endif


