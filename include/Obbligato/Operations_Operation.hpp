#pragma once
#ifndef Obbligato_Operations_Operation_hpp
#define Obbligato_Operations_Operation_hpp

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
#include "Obbligato/Operations_OperationBase.hpp"
#include <memory.h>

namespace Obbligato { namespace Operations {

class Operation : public OperationBase
{
    OperationID m_op_id;
    Net::HandlerPtr m_net_handler;
    std::unique_ptr<TargetPtrVector> m_targets;
    float m_percent_done;
    std::unique_ptr<OperationIDBaseMap> m_sub_operations_map;
    std::unique_ptr<OperationBasePtrQueue> m_sub_operations_queue;
    OperationBasePtr m_current_sub_operation;

public:
    Operation();

    virtual ~Operation();

    virtual Net::HandlerPtr net_handler() const;
    virtual void set_net_handler( Net::HandlerPtr );
    virtual void set_operation_id(OperationID);
    virtual OperationID operation_id() const;
    virtual void operation_add_sub_operation( OperationID, OperationBasePtr );
    virtual OperationBasePtr operation_current() const;
    virtual void operation_add_target( TargetPtr );
    virtual void dump(std::ostream &) const;

    virtual void notify_targets_operation_started();
    virtual void notify_targets_operation_completed();
    virtual void notify_targets_operation_in_progress(float percent_done);
    virtual void notify_targets_operation_timeout();
    virtual void notify_targets_operation_error(std::string error_info);
    virtual void notify_targets_operation_warning(std::string warning_info);
    virtual void notify_targets_operation_aborted(std::string why);

    virtual void operation_start();
    virtual bool operation_is_in_progress() const;
    virtual float operation_progress_in_percent() const;
    virtual bool operation_is_complete() const;
    virtual void operation_abort(std::string why);

    virtual void requested_operation_started( OperationID operation_id );
    virtual void requested_operation_completed( OperationID operation_id );
    virtual void requested_operation_in_progress( OperationID operation_id, float percent );
    virtual void requested_operation_timeout( OperationID operation_id );
    virtual void requested_operation_error( OperationID operation_id, std::string error_info );
    virtual void requested_operation_warning( OperationID operation_id, std::string warning_info );
    virtual void requested_operation_aborted( OperationID operation_id, std::string why );

};


inline bool operator < ( OperationPtr const &a, OperationPtr const &b )
{
    return Operation::compare(a,b) < 0;
}

inline bool operator <= ( OperationPtr const &a, OperationPtr const &b )
{
    return Operation::compare(a,b) <= 0;
}

inline bool operator > ( OperationPtr const &a, OperationPtr const &b )
{
    return Operation::compare(a,b) > 0;
}

inline bool operator >= ( OperationPtr const &a, OperationPtr const &b )
{
    return Operation::compare(a,b) >= 0;
}

inline bool operator == ( OperationPtr const &a, OperationPtr const &b )
{
    return Operation::compare(a,b) == 0;
}

inline bool operator != ( OperationPtr const &a, OperationPtr const &b )
{
    return Operation::compare(a,b) != 0;
}


} }

#endif


