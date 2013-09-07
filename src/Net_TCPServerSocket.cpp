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
#include "Obbligato/Net_TCPServerSocket.hpp"
#include "Obbligato/Net.hpp"

namespace Obbligato {
namespace Net {

TCPServerSocket::TCPServerSocket(Address const &local_addr) {
    initialize_sockets();

    do {
        m_fd = ::socket(local_addr.get_sa_family(), SOCK_STREAM, 0);
    } while (m_fd == INVALID_SOCKET && (errno == EINTR || errno == EAGAIN));

    if (m_fd != INVALID_SOCKET) {
        int on = 1;
        if (::setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&on,
                         sizeof(on)) == 0 &&
            ::bind(m_fd, local_addr.get_sockaddr(), local_addr.get_addrlen()) ==
                0 &&
            ::listen(m_fd, 128) == 0) {
            set_socket_nonblocking(m_fd);
        } else {
            close();
        }
    }
}

TCPServerSocket::~TCPServerSocket() {
#ifdef _WIN32
    ::closesocket(m_fd);
#else
    ::close(m_fd);
#endif
    m_fd = INVALID_SOCKET;
}

void TCPServerSocket::close() {
#ifdef _WIN32
    ::closesocket(m_fd);
#else
    ::close(m_fd);
#endif
    m_fd = INVALID_SOCKET;
}

SOCKET TCPServerSocket::accept(Address &remote) {
    sockaddr_storage remote_addr;
    socklen_t remote_addr_len = sizeof(remote_addr);

    SOCKET clientfd;
    do {
        clientfd = ::accept(fd(), (sockaddr *)&remote_addr, &remote_addr_len);
    } while (clientfd != INVALID_SOCKET && (errno == EAGAIN || errno == EINTR));

    if (clientfd != INVALID_SOCKET) {
        remote = Address(remote_addr);
    }
    return clientfd;
}

SOCKET TCPServerSocket::fd() const { return m_fd; }
}
}
