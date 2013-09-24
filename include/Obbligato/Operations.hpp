#pragma once
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
#include "Obbligato/Operations_Operation.hpp"
#include "Obbligato/Operations_NotificationTarget.hpp"

namespace Obbligato {
namespace Operations {
namespace Experiment {

#if 0
inline std::ostream & operator << ( std::ostream &o, OperationID const &opid ) {
    o << opid.second;
    return o;
}

template <typename T>
void operation_tick(T&op) {
    std::cout << "Default operation_tick" << std::endl;
}

template <typename T>
void operation_start(T&op) {
    std::cout << "Default operation_start" << std::endl;
}

template <typename T>
void operation_abort(T&op) {
    std::cout << "Default operation_abort" << std::endl;
}

template <typename T>
void operation_started(T&op, OperationID opid ) {
    std::cout << "Default operation_started: " << opid << std::endl;
}

template <typename T>
void operation_aborted(T&op, OperationID opid ) {
    std::cout << "Default operation_aborted: " << opid << std::endl;
}

template <typename T>
void operation_in_progress(T&op, OperationID opid, int permil ) {
    std::cout << "Default operation_in_progress: " << opid << " permil:" << permil << std::endl;
}

template <typename T>
void operation_completed(T&op, OperationID opid ) {
    std::cout << "Default operation_completed: " << opid << std::endl;
}

template <typename T>
void operation_timeout(T&op, OperationID opid ) {
    std::cout << "Default operation_timeout: " << opid << std::endl;
}

template <typename T>
void operation_error(T&op, OperationID opid ) {
    std::cout << "Default operation_error: " << opid << std::endl;
}

class MyOperation {
public:
    template <typename T>
    MyOperation(T x) : self_(std::make_shared<MyOperationImpl<T>>(std::move(x))) {}

    friend void operation_tick( MyOperation &op, Time::timestamp_in_microseconds t ) {
        op.self_->operation_tick_(t);
    }

    friend void operation_start( MyOperation &op ) {
        op.self_->operation_start_();
    }

    friend void operation_abort( MyOperation &op ) {
        op.self_->operation_abort_();
    }

    friend void operation_started( MyOperation &op, OperationID opid ) {
        op.self_->operation_started_( opid );
    }

    friend void operation_aborted( MyOperation &op, OperationID opid ) {
        op.self_->operation_aborted_( opid );
    }

    friend void operation_in_progress( MyOperation &op, OperationID opid, int permil ) {
        op.self_->operation_in_progress_( opid, permil );
    }

    friend void operation_completed( MyOperation &op, OperationID opid ) {
        op.self_->operation_completed_( opid );
    }

    friend void operation_timeout( MyOperation &op, OperationID opid ) {
        op.self_->operation_timeout_(opid);
    }

    friend void operation_error( MyOperation &op, OperationID opid ) {
        op.self_->operation_error_(opid);
    }

    struct MyOperationConcept {
        std::shared_ptr<MyOperationConcept> parent;
        std::vector< std::shared_ptr<MyOperationConcept> > child_items;
        static std::atomic<int> opid_counter;

        MyOperationConcept() = default;
        MyOperationConcept( std::shared_ptr<MyOperationConcept> &p ) : parent(p) {
        }

        virtual ~MyOperationConcept() = default;

        virtual void operation_tick_( Time::timestamp_in_microseconds t) = 0;
        virtual void operation_start_() = 0;
        virtual void operation_abort_() = 0;
        virtual void operation_started_( OperationID opid ) = 0;
        virtual void operation_aborted_( OperationID opid ) = 0;
        virtual void operation_in_progress_( OperationID opid, int permil ) = 0;
        virtual void operation_completed_( OperationID opid ) = 0;
        virtual void operation_timeout_( OperationID opid ) = 0;
        virtual void operation_error_( OperationID opid ) = 0;
    };

    template <typename T>
    struct MyOperationImpl : public MyOperationConcept {
        T op;
        OperationID opid;

        MyOperationImpl( T x ) : op(std::move(x)), opid(OperationID(0,opid_counter++)) {}
        MyOperationImpl( T x, std::shared_ptr<MyOperationConcept> p )
          : MyOperationConcept(p), op(std::move(x))
          , opid(OperationID(0,opid_counter++)) {}

        void operation_tick_( Time::timestamp_in_microseconds t) {
            for( auto &i : child_items ) {
                i->operation_tick_( t );
            }
            operation_tick( t );
        }

        void operation_start_() {
            operation_start( op );
            if( parent ) {
                parent->operation_started_( opid );
            }
        }

        void operation_abort_() {
            for( auto &i : child_items ) {
                i->operation_abort_();
            }
            operation_abort( op );
            if( parent ) {
                parent->operation_aborted_( opid );
            }
        }

        void operation_started_( OperationID opid ) {
            operation_started( op, opid );
        }

        void operation_aborted_( OperationID opid ) {
            operation_aborted( op, opid );
        }

        void operation_in_progress_( OperationID opid, int permil ) {
            operation_in_progress(op, opid, permil );
        }

        void operation_completed_( OperationID opid ) {
            operation_completed(op,opid);
        }

        void operation_timeout_( OperationID opid ) {
            operation_timeout( op, opid );
        }

        void operation_error_( OperationID opid ) {
            operation_error( op, opid );
        }
    };


    template <typename T>
    void push_back( T &&v ) {
        self_->child_items.push_back( std::shared_ptr<MyOperationConcept>( new MyOperationImpl<T>(std::move(v),self_)) );
    }

private:
    std::shared_ptr<MyOperationConcept> self_;
};

void operation_start(int x) {
    std::cout << "int operation_start" << std::endl;
}

inline void fff() {
    MyOperation x = "Master";

    x.push_back( std::string("Jeff") );
    x.push_back( 10 );

    operation_start( x );
    operation_tick(x,10);
    operation_abort(x);
}

#endif
}
}
}
