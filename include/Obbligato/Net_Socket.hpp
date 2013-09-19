#pragma once
#ifndef Obbligato_Net_Socket_hpp
#define Obbligato_Net_Socket_hpp

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
#include "Obbligato/Time.hpp"
#include "Obbligato/Time_Ticker.hpp"
#include "Obbligato/SharedPtr.hpp"
#include "Obbligato/Net_Address.hpp"

namespace Obbligato {
namespace Net {

class Socket : public Time::Ticker {
  public:

    Socket() {}

    /// Close and destroy the socket
    virtual ~Socket() {}

    /// Returns true if the object is ready for business
    virtual bool is_open() const = 0;

    /// Close the socket
    virtual void close() = 0;

    /// get the current file descriptor of the socket
    virtual SOCKET fd() const = 0;

    /// notify passage of time
    virtual void tick(Timestamp) = 0;
};

typedef shared_ptr<Socket> SocketPtr;

typedef std::vector<SocketPtr> SocketPtrVector;

bool initialize_sockets();

inline SOCKET get_fd(Socket const &s) { return s.fd(); }

inline SOCKET get_fd(SocketPtr const &s) { return s->fd(); }

inline SOCKET get_fd(SOCKET fd) { return fd; }

Address get_local_address(SOCKET fd);

template <typename SocketT> inline Address get_local_address(SocketT &s) {
    return get_local_address(get_fd(s));
}

Address get_remote_address(SOCKET fd);

template <typename SocketT> inline Address get_remote_address(SocketT &s) {
    return get_remote_address(get_fd(s));
}

void set_socket_blocking(SOCKET fd);

template <typename SocketT> inline void set_socket_blocking(SocketT &s) {
    set_socket_blocking(get_fd(s));
}

void set_socket_nonblocking(SOCKET fd);

template <typename SocketT> inline void set_socket_nonblocking(SocketT &s) {
    set_socket_nonblocking(get_fd(s));
}
}
}

#endif
