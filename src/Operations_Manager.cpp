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
#include "Obbligato/Operations_Manager.hpp"
#include "Obbligato/Operations_Operation.hpp"

namespace Obbligato { namespace Operations {


Manager::Manager( Net::HandlerPtr net_handler ) :
    m_net_handler( net_handler ),
    m_active_operations()
{
}

OperationID Manager::operation_start(OperationBasePtr op)
{
    OperationID opid = next_operation_id();

    op->set_operation_id( opid );
    op->set_net_handler( m_net_handler );
    op->operation_start();
    m_active_operations[opid] = op;
    return opid;
}


void Manager::operation_abort(OperationID op_id, std::string why)
{
    OperationIDBaseMap::iterator i = m_active_operations.find(op_id);
    if( i!=m_active_operations.end() )
    {
        OperationBasePtr op = i->second;
        if( op->operation_is_in_progress() )
        {
            op->operation_abort(why);
        }
    }
}

void Manager::operation_abort_all(std::string why)
{
    for( OperationIDBaseMap::iterator i=m_active_operations.begin(); i!=m_active_operations.end(); ++i )
    {
        OperationBasePtr op = i->second;
        if( op->operation_is_in_progress() )
        {
            op->operation_abort(why);
        }
    }
}

size_t Manager::operation_count() const
{
    return m_active_operations.size();
}

void Manager::prune_inactive_operations()
{
    for( OperationIDBaseMap::iterator i=m_active_operations.begin(); i!=m_active_operations.end(); )
    {
        OperationBasePtr op = i->second;
        if( !op->operation_is_in_progress() )
        {
            m_active_operations.erase(i++);
        }
        else
        {
            ++i;
        }
    }
}

void Manager::tick(Timestamp)
{
    prune_inactive_operations();
}


void Manager::requested_operation_started( OperationID operation_id )
{
    (void)operation_id;
}

void Manager::requested_operation_completed( OperationID operation_id )
{
    (void)operation_id;

}

void Manager::requested_operation_in_progress( OperationID operation_id, float percent )
{
    (void)operation_id;
    (void)percent;
}

void Manager::requested_operation_timeout( OperationID operation_id )
{
    (void)operation_id;
}

void Manager::requested_operation_error( OperationID operation_id, std::string error_info )
{
    (void)operation_id;
    (void)error_info;
}

void Manager::requested_operation_warning( OperationID operation_id, std::string warning_info )
{
    (void)operation_id;
    (void)warning_info;
}

void Manager::requested_operation_aborted( OperationID operation_id, std::string why )
{
    (void)operation_id;
    (void)why;

}



}}

