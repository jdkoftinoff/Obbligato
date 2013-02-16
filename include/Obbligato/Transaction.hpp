#pragma once
#ifndef Obbligato_Transaction_hpp
#define Obbligato_Transaction_hpp

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
#include "Obbligato/Action.hpp"
#include "Obbligato/Stack.hpp"
#include "Obbligato/SharedPtr.hpp"

namespace Obbligato
{
    /// A Transaction performs some action upon construction and un-does the action on destruction unless successful() is called before destruction.
    class Transaction
    {
        /// The Action object which knows how to do something and undo something.
        Action m_action;

        /// The success flag
        bool m_success;
        
    public:

        /// Constructor assigns members and calls do_action
        Transaction( Action action ) :
            m_action( action ),
            m_success(false)
        {
            m_action.do_action();
        }

        /// mark the transaction as successful
        void successful()
        {
            m_success = true;
        }
        
        /// If the transaction was not successful due to exception or dependent transaction failing, un-do the action.
        ~Transaction()
        {
            if( !m_success )
            {
                m_action.undo_action();
            }
        }
    };

    /// Create a Transaction object given an Action object
    inline
    Transaction
    make_transaction(
                     Action action
                     )
    {
        return Transaction(
                    action
                    );
    }


    /// Create a Transaction object given 'do' and 'undo' functors.
    template <typename DoFunctorT,typename UndoFunctorT>
    inline
    Transaction
    make_transaction(
                     DoFunctorT do_functor,
                     UndoFunctorT undo_functor
                     )
    {
        return Transaction(
                    Action(do_functor,undo_functor)
                    );
    }


    /// Create a Transaction object given 'do' and 'undo' methods of an object.
    template< typename ObjClassT >
    inline
    Transaction
    make_transaction (
         ObjClassT &obj,
         void ( ObjClassT::*doit_method ) (),
         void ( ObjClassT::*undoit_method ) ()
       )
       {
         return Transaction(
                     make_action(
                                 obj,
                                 doit_method,
                                 undoit_method
                                 )
                            );
       }

}

#endif


