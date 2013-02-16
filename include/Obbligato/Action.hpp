#pragma once
#ifndef Obbligato_Action_hpp
#define Obbligato_Action_hpp

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
#include "Obbligato/Function.hpp"
#include "Obbligato/SharedPtr.hpp"

namespace Obbligato
{
    /// Action is a template which inherits from ActionBase and is templatized to contain function objects which perform the do_action and undo_action
    class Action
    {
    public:

        /// The functor type for doing something
        typedef function<void()> do_functor_type;

        /// The functor type for undoing something
        typedef function<void()> undo_functor_type;

    private:
        /// The function object that does something
        do_functor_type m_do_functor;

        /// The function object that un-does something
        undo_functor_type m_undo_functor;

    public:        
        /// Constructor takes function objects for doing and undoing
        Action( do_functor_type do_functor = do_functor_type(), undo_functor_type undo_functor = undo_functor_type() )
            : m_do_functor( do_functor ),
              m_undo_functor( undo_functor )
        {
        }

        /// Copy constructor
        Action( Action const &other)
            : m_do_functor( other.m_do_functor ),
              m_undo_functor( other.m_undo_functor )
        {
        }

        /// Assignment operator
        Action const & operator = ( Action other )
        {
            using namespace std;
            swap( m_do_functor, other.m_do_functor );
            swap( m_undo_functor, other.m_undo_functor );
            return *this;
        }

        /// Destructor
        ~Action()
        {
        }

        /// run the do functor
        void do_action()
        {
            m_do_functor();
        }


        /// run the undo functor
        void undo_action()
        {
            m_undo_functor();
        }
    };

    /// Helper function to create the appropriate Action object based on a do_functor and undo_functor
    template <typename DoFunctorT,typename UndoFunctorT>
    inline Action make_action(
                DoFunctorT do_functor,
                UndoFunctorT undo_functor
                )
    {
        return Action(
                      do_functor,
                      undo_functor
                      );
    }

    /// Helper function to create the appropriate Action object that calls methods into an object for do and undo
    template <typename ObjT>
    inline
    Action make_action(
                ObjT &obj,
                void ( ObjT::*do_method ) (),
                void ( ObjT::*undo_method ) ()
                )
    {
        return Action(
                      bind( do_method, &obj ),
                      bind( undo_method, &obj )
                      );
    }

}

#endif


