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
#include "Obbligato/Service_Posix.hpp"
#include "Obbligato/Form.hpp"

#if defined(__linux__) || defined(__APPLE__)

namespace Obbligato {
namespace Service {

static ::std::string pid_file_name;
static int pid_fd = -1;

void drop_root(::std::string uid_name) {
    uid_t new_uid = 0;
    uid_t new_gid = 0;
    setsid();
    if (getuid() == 0) {
        struct passwd *pw = getpwnam(uid_name.c_str());
        if (pw) {
            new_uid = pw->pw_uid;
            new_gid = pw->pw_gid;
        } else {
            new_uid = (uid_t)strtol(uid_name.c_str(), 0, 10);
            pw = getpwuid(new_uid);
            if (pw) {
                new_gid = pw->pw_gid;
            }
        }
        if (setegid(new_gid) < 0 || seteuid(new_uid) < 0) {
            perror("error setting euid/egid");
            exit(1);
        }
    }
}

void daemonize(bool real_daemon, ::std::string identity, ::std::string home_dir,
               ::std::string pid_file, ::std::string new_uid) {
    /* remember the pid file name */
    pid_file_name = pid_file;
    if (home_dir.length() > 0) {
        mkdir(home_dir.c_str(), 0750);
        if (chdir(home_dir.c_str()) != 0) {
            perror("error setting home dir");
            exit(1);
        }
    }
    if (real_daemon) {
        pid_t r;
        do {
            r = fork();
        } while (r == -1 && errno == EINTR);

        switch (r) {
        case -1:
            perror("fork() failed");
            exit(1);
        case 0:
            break;
        default:
            _exit(0);
            break;
        }
        ;
    }
    if (new_uid.length() > 0) {
        drop_root(new_uid);
    }
    if (real_daemon) {
        int fd;
        for (fd = 0; fd < getdtablesize(); ++fd) {
            close(fd);
        }
        fd = open("/dev/null", O_RDWR);
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
    }
    umask(027);
    if (pid_file_name.length() > 0) {
        pid_fd =
            open(pid_file_name.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0640);
        if (pid_fd >= 0) {
            char tmpbuf[64];
            if (lockf(pid_fd, F_TLOCK, 0) < 0) {
                ob_log_error("%s: Unable to lock pid file: %s",
                             identity.c_str(), pid_file_name.c_str());
                abort();
            }
            sprintf(tmpbuf, "%ld\n", (long)getpid());
            size_t len = strlen(tmpbuf);
            if (write(pid_fd, tmpbuf, len) != ssize_t(len)) {
                ob_log_error("%s: Error writing pid file: %s", identity.c_str(),
                             pid_file_name.c_str());
                abort();
            }
            atexit(daemon_end);
        } else {
            ob_log_error("%s: Error creating pid file: %s", identity.c_str(),
                         pid_file_name.c_str());
            abort();
        }
    }
    prepare_child_start();
}

void prepare_child_start() {
    // TODO: setup signal handlers
}

pid_t daemon_fork(void) {
    pid_t r;
    do {
        r = fork();
    } while (r == -1 && errno == EINTR);

    switch (r) {
    case -1:
        perror("fork() failed");
        exit(1);
    case 0:
        // in child process
        prepare_child_start();
        break;
    default:
        // in parent process
        break;
    }
    ;
    return r;
}

void daemon_end(void) { _exit(0); }
}
}
#else
namespace Obbligato {

const char *service_posix_file = __FILE__;
}

#endif
