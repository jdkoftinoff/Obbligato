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
#include "Obbligato/Net_Socket.hpp"
#include "Obbligato/Net_AddressList.hpp"

namespace Obbligato {
namespace Net {

class TCPSocket : public Socket {
  private:
    socket_fd_t m_fd;

  public:

    TCPSocket(Address const &local_address);

    TCPSocket(socket_fd_t accepted_fd);

    TCPSocket(TCPSocket &&other)
        : Socket(std::move(other)), m_fd(std::move(other.m_fd)) {}

    TCPSocket const &operator=(TCPSocket &&other) {
        Socket::operator=(std::move(other));
        m_fd = std::move(m_fd);
        return *this;
    }

    virtual ~TCPSocket();

    virtual bool blocking_connect_list(AddressList const &remote_address_list);

    virtual bool blocking_connect(Address const &remote_address);

    virtual bool is_open() const { return m_fd != INVALID_SOCKET; }

    virtual void close();

    virtual ssize_t send(void const *data, ssize_t len);

    virtual ssize_t recv(void *data, ssize_t len);

    virtual socket_fd_t fd() const { return m_fd; }

    virtual void tick(Timestamp);
};

inline TCPSocket make_tcpsocket(Address local_address) {
    return TCPSocket(local_address);
}
}
}
