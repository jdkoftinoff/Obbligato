#pragma once
#ifndef Obbligato_Operations_Manager_hpp
#define Obbligato_Operations_Manager_hpp

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
#include "Obbligato/Operations_ManagerBase.hpp"
#include "Obbligato/Operations_Operation.hpp"

namespace Obbligato { namespace Operations {


class Manager : public ManagerBase
{
    OperationIDBaseMap m_active_operations;

    virtual void prune_inactive_operations();

public:
    Manager();

    virtual OperationID operation_start(OperationBasePtr op);
    virtual void operation_abort(OperationID op_id, std::string why);
    virtual void operation_abort_all( std::string why );
    virtual size_t operation_count() const;
    virtual void dump( std::ostream &) const;

    virtual void tick(Timestamp timestamp);

    virtual void requested_operation_started( OperationID operation_id );
    virtual void requested_operation_completed( OperationID operation_id );
    virtual void requested_operation_in_progress( OperationID operation_id, float percent );
    virtual void requested_operation_timeout( OperationID operation_id );
    virtual void requested_operation_error( OperationID operation_id, std::string error_info );
    virtual void requested_operation_warning( OperationID operation_id, std::string warning_info );
    virtual void requested_operation_aborted( OperationID operation_id, std::string why );

};


} }

#endif


