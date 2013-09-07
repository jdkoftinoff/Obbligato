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
#include "Obbligato/IOStream.hpp"

namespace Obbligato {
namespace Operations {

Operation::Operation(std::string const &operation_description)
    : OperationBase(operation_description), m_operation_id(),
      m_progress_in_permil(-1), m_current_sub_operation(), m_primary_target(),
      m_targets(), m_sub_operations_map() {}

Operation::~Operation() { notify_targets_operation_completed(); }

void Operation::set_operation_id(OperationID op_id) { m_operation_id = op_id; }

OperationID Operation::operation_id() const { return m_operation_id; }

void Operation::operation_add_sub_operation(OperationID sub_op_id,
                                            OperationBasePtr sub_op) {
    sub_op->operation_set_primary_target(shared_from_this());

    m_sub_operations_map[sub_op_id] = sub_op;
}

OperationBasePtr Operation::operation_current() const {
    if (m_current_sub_operation) {
        return m_current_sub_operation;
    } else {
        return OperationBasePtr();
    }
}

void Operation::operation_add_target(NotificationTargetPtr t) {
    m_targets.push_back(t);
}

void Operation::operation_set_primary_target(NotificationTargetPtr t) {
    m_primary_target = t;
}

void Operation::dump(std::ostream &os) const {
    using namespace ::Obbligato::IOStream;
    OStreamStateSave osave(os);

    os << title_fmt("Operation") << fmt(this) << std::endl;
    os << label_fmt("operation_description") << fmt(operation_description())
       << std::endl;
    os << label_fmt("progress_in_permil") << fmt(operation_progress_in_permil())
       << std::endl;
    os << label_fmt("operation_id") << "( " << fmt(m_operation_id.first) << ","
       << fmt(m_operation_id.second) << " )" << std::endl;
    os << label_fmt("primary_target") << fmt(m_primary_target.get())
       << std::endl;
    os << label_fmt("current_sub_operation")
       << fmt(m_current_sub_operation.get());
    if (m_current_sub_operation) {
        os << "( " << fmt(m_current_sub_operation->operation_id().first) << ","
           << fmt(m_current_sub_operation->operation_id().second) << " )"
           << ":" << m_current_sub_operation->operation_description()
           << std::endl;
    }
    os << std::endl;

    os << label_fmt("targets");
    for (NotificationTargetPtrVector::const_iterator i = m_targets.begin();
         i != m_targets.end(); ++i) {
        NotificationTarget const *p = (*i).get();
        os << fmt(p) << " ";
    }
    os << std::endl;

    os << label_fmt("sub_operation count");
    os << fmt(m_sub_operations_map.size());
    os << std::endl;

    os << label_fmt("sub_operations") << std::endl;
    for (OperationIDBaseMap::const_iterator i = m_sub_operations_map.begin();
         i != m_sub_operations_map.end();) {
        OperationBasePtr op = i->second;
        op->dump(os);
    }
}

void Operation::operation_start() {}

bool Operation::operation_is_in_progress() const {
    return m_progress_in_permil != 0 && m_progress_in_permil != 1000;
}

int Operation::operation_progress_in_permil() const {
    return m_progress_in_permil;
}

bool Operation::operation_is_complete() const {
    return m_progress_in_permil == 1000;
}

void Operation::operation_abort(std::string const &why) {
    // abort
    m_progress_in_permil = 0;
    notify_targets_operation_aborted(why);
}

void Operation::prune_inactive_operations() {
    for (OperationIDBaseMap::iterator i = m_sub_operations_map.begin();
         i != m_sub_operations_map.end();) {
        OperationBasePtr op = i->second;
        if (!op->operation_is_in_progress()) {
            m_sub_operations_map.erase(i++);
        } else {
            ++i;
        }
    }
}

void Operation::tick(Timestamp) { prune_inactive_operations(); }

Timestamp Operation::ticker_next_tick_time(Timestamp curtime) {
    return curtime + ticker_get_time_per_tick_in_ms();
}

void Operation::notify_targets_operation_started() {
    if (m_primary_target) {
        m_primary_target->requested_operation_started(m_operation_id);
    }

    for (NotificationTargetPtrVector::iterator i = m_targets.begin();
         i != m_targets.end(); ++i) {
        NotificationTarget *t = i->get();
        t->requested_operation_started(m_operation_id);
    }
}

void Operation::notify_targets_operation_completed() {
    if (m_primary_target) {
        m_primary_target->requested_operation_completed(m_operation_id);
    }
    for (NotificationTargetPtrVector::iterator i = m_targets.begin();
         i != m_targets.end(); ++i) {
        NotificationTarget *t = i->get();
        t->requested_operation_completed(m_operation_id);
    }
}

void Operation::notify_targets_operation_in_progress(float percent_done) {
    if (m_primary_target) {
        m_primary_target->requested_operation_in_progress(m_operation_id,
                                                          percent_done);
    }
    for (NotificationTargetPtrVector::iterator i = m_targets.begin();
         i != m_targets.end(); ++i) {
        NotificationTarget *t = i->get();
        t->requested_operation_in_progress(m_operation_id, percent_done);
    }
}

void Operation::notify_targets_operation_timeout() {
    if (m_primary_target) {
        m_primary_target->requested_operation_timeout(m_operation_id);
    }
    for (NotificationTargetPtrVector::iterator i = m_targets.begin();
         i != m_targets.end(); ++i) {
        NotificationTarget *t = i->get();
        t->requested_operation_timeout(m_operation_id);
    }
}

void Operation::notify_targets_operation_error(std::string const &error_info) {
    if (m_primary_target) {
        m_primary_target->requested_operation_error(m_operation_id, error_info);
    }
    for (NotificationTargetPtrVector::iterator i = m_targets.begin();
         i != m_targets.end(); ++i) {
        NotificationTarget *t = i->get();
        t->requested_operation_error(m_operation_id, error_info);
    }
}

void
Operation::notify_targets_operation_warning(std::string const &warning_info) {
    if (m_primary_target) {
        m_primary_target->requested_operation_warning(m_operation_id,
                                                      warning_info);
    }
    for (NotificationTargetPtrVector::iterator i = m_targets.begin();
         i != m_targets.end(); ++i) {
        NotificationTarget *t = i->get();
        t->requested_operation_warning(m_operation_id, warning_info);
    }
}

void Operation::notify_targets_operation_aborted(std::string const &why) {
    if (m_primary_target) {
        m_primary_target->requested_operation_aborted(m_operation_id, why);
    }
    for (NotificationTargetPtrVector::iterator i = m_targets.begin();
         i != m_targets.end(); ++i) {
        NotificationTarget *t = i->get();
        t->requested_operation_aborted(m_operation_id, why);
    }
}

void Operation::requested_operation_started(OperationID operation_id) {
    (void)operation_id;
}

void Operation::requested_operation_completed(OperationID operation_id) {
    (void)operation_id;
}

void Operation::requested_operation_in_progress(OperationID operation_id,
                                                int permil) {
    (void)operation_id;
    (void)permil;
}

void Operation::requested_operation_timeout(OperationID operation_id) {
    (void)operation_id;
}

void Operation::requested_operation_error(OperationID operation_id,
                                          std::string const &error_info) {
    (void)operation_id;
    (void)error_info;
}

void Operation::requested_operation_warning(OperationID operation_id,
                                            std::string const &warning_info) {
    (void)operation_id;
    (void)warning_info;
}

void Operation::requested_operation_aborted(OperationID operation_id,
                                            std::string const &why) {
    (void)operation_id;
    (void)why;
}
}
}
