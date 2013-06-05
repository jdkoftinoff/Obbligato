#pragma once
#ifndef Obbligato_Time_Ticker_hpp
#define Obbligato_Time_Ticker_hpp

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
#include "Obbligato/Time_TimeVal.hpp"
#include "Obbligato/Time_Linux.hpp"
#include "Obbligato/Time_MacOSX.hpp"
#include "Obbligato/Time_Win32.hpp"

namespace Obbligato { namespace Time {

class Ticker
{
    Timestamp m_last_tick_time;
    Timestamp m_next_tick_time;
    uint32_t m_time_per_tick_in_ms;

public:
    Ticker(uint32_t time_in_ms_per_tick=100);
    virtual ~Ticker();

    void ticker_set_time_per_tick_in_ms( uint32_t time_in_ms )
    {
        m_time_per_tick_in_ms = time_in_ms;
        m_next_tick_time = m_last_tick_time + time_in_ms;
    }

    uint32_t ticker_get_time_per_tick_in_ms() const
    {
        return m_time_per_tick_in_ms;
    }

    void ticker_tick( Timestamp timestamp );

    Timestamp ticker_next_tick_time()
    {
        return m_next_tick_time;
    }

    virtual void tick(Timestamp timestamp ) = 0;
};

}}
#endif


