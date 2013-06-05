#pragma once
#ifndef Obbligato_Operation_decl_hpp
#define Obbligato_Operation_decl_hpp

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
#include "Obbligato/Net_ReactorBase.hpp"
#include "Obbligato/Net_Handler.hpp"
#include "Obbligato/Atomic.hpp"

namespace Obbligato { namespace Operations {

class ManagerBase;
class NotificationTarget;
class OperationBase;
class Operation;

typedef std::shared_ptr<ManagerBase> ManagerBasePtr;

typedef uint32_t SequenceID;
typedef std::pair< OperationBase *, uint32_t > OperationID;

typedef std::shared_ptr<NotificationTarget> NotificationTargetPtr;
typedef std::vector<NotificationTargetPtr> NotificationTargetPtrVector;

typedef std::shared_ptr<OperationBase> OperationBasePtr;
typedef std::vector<OperationBasePtr> OperationBasePtrVector;
typedef std::queue<OperationBasePtr> OperationBasePtrQueue;
typedef std::set< OperationBasePtr > OperationBasePtrSet;
typedef std::map<OperationID,OperationBasePtr> OperationIDBaseMap;

typedef std::shared_ptr<Operation> OperationPtr;
typedef std::vector<OperationBasePtr> OperationPtrVector;
typedef std::queue<OperationBasePtr> OperationPtrQueue;

typedef std::map<SequenceID,OperationBasePtr> OperationPtrSequenceIDMap;
typedef std::map<OperationID,OperationBasePtr> OperationIDMap;



} }

#endif


