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
#include "Obbligato/Net_TCPSocket.hpp"
#include "Obbligato/Net.hpp"

namespace Obbligato {
namespace Net {

TCPSocket::TCPSocket(Address const &local_addr) {
    initialize_sockets();

    do {
        m_fd = ::socket(local_addr.get_sa_family(), SOCK_STREAM, 0);
    } while (m_fd == INVALID_SOCKET && (errno == EINTR || errno == EAGAIN));

    if (m_fd != INVALID_SOCKET) {
        set_socket_nonblocking(m_fd);
    }
}

TCPSocket::TCPSocket(socket_fd_t accepted_fd) : m_fd(accepted_fd) {}

TCPSocket::~TCPSocket() {
#ifdef _WIN32
    ::closesocket(m_fd);
#else
    ::close(m_fd);
#endif
    m_fd = INVALID_SOCKET;
}

void TCPSocket::close() {
#ifdef _WIN32
    ::closesocket(m_fd);
#else
    ::close(m_fd);
#endif
    m_fd = INVALID_SOCKET;
}

bool TCPSocket::blocking_connect_list(AddressList const &remote_address_list) {
    bool r = false;

    for (AddressList::const_iterator i = remote_address_list.begin();
         i != remote_address_list.end(); ++i) {
        if (blocking_connect(*i)) {
            r = true;
            break;
        }
    }

    return r;
}

bool TCPSocket::blocking_connect(Address const &dest) {
    bool r = false;
    int e;

    set_socket_blocking(fd());

    do {
        e = ::connect(m_fd, dest.get_sockaddr(), dest.get_addrlen());
    } while (e < 0 && (errno == EAGAIN || errno == EINTR));

    if (e == 0) {
        r = true;
    }
    set_socket_nonblocking(fd());
    return r;
}

ssize_t TCPSocket::send(void const *data, ssize_t len) {
    ssize_t r = -1;

    do {
        r = ::send(m_fd, (const char *)data, len, 0);

    } while (r < 0 && (errno == EINTR));

    return r;
}

ssize_t TCPSocket::recv(void *data, ssize_t len) {
    ssize_t r = -1;

    do {
        r = ::recv(m_fd, (char *)data, len, 0);

    } while (r < 0 && (errno == EINTR));

    return r;
}
}
}
