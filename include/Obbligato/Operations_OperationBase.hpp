#pragma once
#ifndef Obbligato_Operations_OperationBase_hpp
#define Obbligato_Operations_OperationBase_hpp

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
#include "Obbligato/Operations_decl.hpp"
#include "Obbligato/Operations_NotificationTarget.hpp"

namespace Obbligato { namespace Operations {


class OperationBase : public NotificationTarget, public Time::Ticker
{
private:
    static Atomic::AtomicInt m_next_operation_id;
    std::string const m_operation_description;

public:
    OperationBase( std::string const &operation_description )
        : m_operation_description(operation_description)
    {
    }
    virtual ~OperationBase() {}

    std::string operation_description() const
    {
        return m_operation_description;
    }

    OperationID next_operation_id()
    {
        return std::make_pair( this, uint32_t(m_next_operation_id++) );
    }

    virtual void set_operation_id(OperationID) = 0;
    virtual OperationID operation_id() const = 0;
    virtual void operation_add_sub_operation( OperationID, OperationBasePtr ) = 0;
    virtual OperationBasePtr operation_current() const = 0;
    virtual void operation_add_target( TargetPtr ) = 0;
    virtual void operation_set_primary_target( TargetPtr ) = 0;
    virtual void dump(std::ostream &) const = 0;

    virtual void notify_targets_operation_started() = 0;
    virtual void notify_targets_operation_completed() = 0;
    virtual void notify_targets_operation_in_progress(float percent_done) = 0;
    virtual void notify_targets_operation_timeout() = 0;
    virtual void notify_targets_operation_error(std::string const &error_info) = 0;
    virtual void notify_targets_operation_warning(std::string const &warning_info) = 0;
    virtual void notify_targets_operation_aborted(std::string const &why) = 0;

    virtual void operation_start() = 0;
    virtual bool operation_is_in_progress() const = 0;
    virtual float operation_progress_in_percent() const = 0;
    virtual bool operation_is_complete() const = 0;
    virtual void operation_abort(std::string const &why) = 0;

    virtual void tick(Timestamp timestamp) = 0;
    virtual Timestamp ticker_next_tick_time(Timestamp curtime) = 0;

    virtual void requested_operation_started( OperationID operation_id ) = 0;
    virtual void requested_operation_completed( OperationID operation_id ) = 0;
    virtual void requested_operation_in_progress( OperationID operation_id, float percent ) = 0;
    virtual void requested_operation_timeout( OperationID operation_id ) = 0;
    virtual void requested_operation_error( OperationID operation_id, std::string const &error_info ) = 0;
    virtual void requested_operation_warning( OperationID operation_id, std::string const &warning_info ) = 0;
    virtual void requested_operation_aborted( OperationID operation_id, std::string const &why ) = 0;

    static int compare(OperationBase const &a, OperationBase const &b )
    {
        if( a.operation_id() < b.operation_id() )
        {
            return -1;
        }
        else if( b.operation_id() < a.operation_id() )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    static int compare(OperationBasePtr const &a, OperationBasePtr const &b )
    {
        if( a->operation_id() < b->operation_id() )
        {
            return -1;
        }
        else if( b->operation_id() < a->operation_id() )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }


};


inline bool operator < ( OperationBase const &a, OperationBase const &b )
{
    return OperationBase::compare(a,b) < 0;
}

inline bool operator <= ( OperationBase const &a, OperationBase const &b )
{
    return OperationBase::compare(a,b) <= 0;
}

inline bool operator > ( OperationBase const &a, OperationBase const &b )
{
    return OperationBase::compare(a,b) > 0;
}

inline bool operator >= ( OperationBase const &a, OperationBase const &b )
{
    return OperationBase::compare(a,b) >= 0;
}

inline bool operator == ( OperationBase const &a, OperationBase const &b )
{
    return OperationBase::compare(a,b) == 0;
}

inline bool operator != ( OperationBase const &a, OperationBase const &b )
{
    return OperationBase::compare(a,b) != 0;
}


inline bool operator < ( OperationBasePtr const &a, OperationBasePtr const &b )
{
    return OperationBase::compare(a,b) < 0;
}

inline bool operator <= ( OperationBasePtr const &a, OperationBasePtr const &b )
{
    return OperationBase::compare(a,b) <= 0;
}

inline bool operator > ( OperationBasePtr const &a, OperationBasePtr const &b )
{
    return OperationBase::compare(a,b) > 0;
}

inline bool operator >= ( OperationBasePtr const &a, OperationBasePtr const &b )
{
    return OperationBase::compare(a,b) >= 0;
}

inline bool operator == ( OperationBasePtr const &a, OperationBasePtr const &b )
{
    return OperationBase::compare(a,b) == 0;
}

inline bool operator != ( OperationBasePtr const &a, OperationBasePtr const &b )
{
    return OperationBase::compare(a,b) != 0;
}


} }

#endif


