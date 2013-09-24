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

namespace Obbligato {
namespace Time {

typedef uint64_t timestamp_in_microseconds;

class TimeVal : public ::timeval {
  public:
    TimeVal() {
        tv_sec = 0;
        tv_usec = 0;
    }

    TimeVal(timeval &other) {
        tv_sec = other.tv_sec;
        tv_usec = other.tv_usec;
    }

    TimeVal(int64_t useconds) { time_in_useconds(useconds); }

    void clear() {
        tv_sec = 0;
        tv_usec = 0;
    }

    int64_t time_in_useconds() const {
        return ((int64_t)tv_sec * 1000000 + tv_usec);
    }

    void time_in_useconds(int64_t usec) {
        tv_sec = static_cast<int32_t>(usec / 1000000);
        tv_usec = static_cast<int32_t>(usec % 1000000);
    }

    TimeVal const &operator=(::timeval const &other) {
        tv_sec = static_cast<int32_t>(other.tv_sec);
        tv_usec = static_cast<int32_t>(other.tv_usec);
        return *this;
    }

    int64_t compare(TimeVal const &other) const {
        return time_in_useconds() - other.time_in_useconds();
    }

    TimeVal const &operator-=(uint32_t useconds) {
        time_in_useconds(time_in_useconds() - useconds);
        return *this;
    }

    TimeVal const &operator-=(TimeVal const &other) {
        time_in_useconds(time_in_useconds() - other.time_in_useconds());
        return *this;
    }

    TimeVal const &operator+=(uint32_t useconds) {
        time_in_useconds(time_in_useconds() + useconds);
        return *this;
    }

    TimeVal const &operator+=(TimeVal const &other) {
        time_in_useconds(time_in_useconds() + other.time_in_useconds());
        return *this;
    }
};

inline bool operator==(TimeVal const &a, TimeVal const &b) {
    return a.compare(b) == 0;
}

inline bool operator!=(TimeVal const &a, TimeVal const &b) {
    return a.compare(b) != 0;
}

inline bool operator<(TimeVal const &a, TimeVal const &b) {
    return a.compare(b) < 0;
}

inline bool operator<=(TimeVal const &a, TimeVal const &b) {
    return a.compare(b) <= 0;
}

inline bool operator>(TimeVal const &a, TimeVal const &b) {
    return a.compare(b) > 0;
}

inline bool operator>=(TimeVal const &a, TimeVal const &b) {
    return a.compare(b) > 0;
}

inline TimeVal operator-(TimeVal const &a, TimeVal const &b) {
    TimeVal r(a);
    r -= b;
    return r;
}

inline TimeVal operator+(TimeVal const &a, TimeVal const &b) {
    TimeVal r(a);
    r += b;
    return r;
}

inline std::ostream &operator<<(std::ostream &s, const ::timeval &v) {
    std::stringstream ss;
    ss << v.tv_sec << "." << std::setfill('0') << std::setw(6) << v.tv_usec;
    s << ss.str();
    return s;
}
}
}
