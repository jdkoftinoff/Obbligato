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
#include "Obbligato/Operations_Operation.hpp"

namespace Obbligato { namespace Operations {

Operation::Operation(std::string const &operation_description) :
    OperationBase(operation_description),
    m_op_id(),
    m_targets(),
    m_percent_done(-1.0f),
    m_sub_operations_map(),
    m_sub_operations_queue(),
    m_current_sub_operation()
{
}

Operation::~Operation()
{
    if( m_targets )
    {
        notify_targets_operation_completed();
    }
}

void Operation::set_operation_id(OperationID op_id )
{
    m_op_id = op_id;
}

OperationID Operation::operation_id() const
{
    return m_op_id;
}

void Operation::operation_add_sub_operation(OperationID sub_op_id, OperationBasePtr sub_op)
{
    if( !m_sub_operations_map )
    {
        m_sub_operations_map.reset(new OperationIDBaseMap());
    }

    sub_op->operation_set_primary_target(shared_from_this());

    (*m_sub_operations_map)[sub_op_id] = sub_op;

    if( !m_sub_operations_queue )
    {
        m_sub_operations_queue.reset( new OperationBasePtrQueue() );
        m_current_sub_operation = sub_op;
    }

    m_sub_operations_queue->push( sub_op );
}

OperationBasePtr Operation::operation_current() const
{
    if( m_current_sub_operation )
    {
        return m_current_sub_operation;
    }
    else
    {
        return OperationBasePtr();
    }
}

void Operation::operation_add_target( TargetPtr t )
{
    if( !m_targets )
    {
        m_targets.reset( new TargetPtrVector() );
    }

    m_targets->push_back( t );
}

void Operation::operation_set_primary_target( TargetPtr t )
{
    m_primary_target = t;
}


void Operation::dump(std::ostream &) const
{

}
    
void Operation::operation_start()
{
}

bool Operation::operation_is_in_progress() const
{
    return m_percent_done!=0.0f && m_percent_done!=100.0f;
}


float Operation::operation_progress_in_percent() const
{
    return m_percent_done;
}


bool Operation::operation_is_complete() const
{
    return m_percent_done==100.0f;
}


void Operation::operation_abort(std::string why)
{
    // abort
    m_percent_done=0.0f;
    notify_targets_operation_aborted(why);
}
    

void Operation::notify_targets_operation_started()
{
    if( m_primary_target )
    {
        m_primary_target->requested_operation_started(m_op_id);
    }
    if( m_targets )
    {
        for( TargetPtrVector::iterator i=m_targets->begin(); i!=m_targets->end(); ++i )
        {
            NotificationTarget *t=i->get();
            t->requested_operation_started(m_op_id);
        }
    }
}

void Operation::notify_targets_operation_completed()
{
    if( m_primary_target )
    {
        m_primary_target->requested_operation_completed(m_op_id);
    }
    if( m_targets )
    {
        for( TargetPtrVector::iterator i=m_targets->begin(); i!=m_targets->end(); ++i )
        {
            NotificationTarget *t=i->get();
            t->requested_operation_completed(m_op_id);
        }
    }
}

void Operation::notify_targets_operation_in_progress(float percent_done)
{
    if( m_primary_target )
    {
        m_primary_target->requested_operation_in_progress(m_op_id,percent_done);
    }
    if( m_targets )
    {
        for( TargetPtrVector::iterator i=m_targets->begin(); i!=m_targets->end(); ++i )
        {
            NotificationTarget *t=i->get();
            t->requested_operation_in_progress(m_op_id,percent_done);
        }
    }
}

void Operation::notify_targets_operation_timeout()
{
    if( m_primary_target )
    {
        m_primary_target->requested_operation_timeout(m_op_id);
    }
    if( m_targets )
    {
        for( TargetPtrVector::iterator i=m_targets->begin(); i!=m_targets->end(); ++i )
        {
            NotificationTarget *t=i->get();
            t->requested_operation_timeout(m_op_id);
        }
    }
}

void Operation::notify_targets_operation_error(std::string error_info)
{
    if( m_primary_target )
    {
        m_primary_target->requested_operation_error(m_op_id,error_info);
    }
    if( m_targets )
    {
        for( TargetPtrVector::iterator i=m_targets->begin(); i!=m_targets->end(); ++i )
        {
            NotificationTarget *t=i->get();
            t->requested_operation_error(m_op_id,error_info);
        }
    }
}

void Operation::notify_targets_operation_warning(std::string warning_info)
{
    if( m_primary_target )
    {
        m_primary_target->requested_operation_warning(m_op_id,warning_info);
    }
    if( m_targets )
    {
        for( TargetPtrVector::iterator i=m_targets->begin(); i!=m_targets->end(); ++i )
        {
            NotificationTarget *t=i->get();
            t->requested_operation_warning(m_op_id,warning_info);
        }
    }
}

void Operation::notify_targets_operation_aborted(std::string why)
{
    if( m_primary_target )
    {
        m_primary_target->requested_operation_aborted(m_op_id,why);
    }
    if( m_targets )
    {
        for( TargetPtrVector::iterator i=m_targets->begin(); i!=m_targets->end(); ++i )
        {
            NotificationTarget *t=i->get();
            t->requested_operation_aborted(m_op_id,why);
        }
    }
}

void Operation::requested_operation_started( OperationID operation_id )
{
    (void)operation_id;
}

void Operation::requested_operation_completed( OperationID operation_id )
{
    (void)operation_id;

}

void Operation::requested_operation_in_progress( OperationID operation_id, float percent )
{
    (void)operation_id;
    (void)percent;
}

void Operation::requested_operation_timeout( OperationID operation_id )
{
    (void)operation_id;
}

void Operation::requested_operation_error( OperationID operation_id, std::string error_info )
{
    (void)operation_id;
    (void)error_info;
}

void Operation::requested_operation_warning( OperationID operation_id, std::string warning_info )
{
    (void)operation_id;
    (void)warning_info;
}

void Operation::requested_operation_aborted( OperationID operation_id, std::string why )
{
    (void)operation_id;
    (void)why;

}


} }

