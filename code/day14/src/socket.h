#ifndef SOCKET_H
#define SOCKET_H

#include "common.h"
#include <string>

class Socket {
public:
    Socket();
    ~Socket();
    DISALLOW_COPY_AND_MOVE(Socket);

    void set_fd(int fd);
    int fd() const;
    std::string get_addr() const;

    RC create();
    RC bind(const char *ip, uint16_t port) const;
    RC listen() const;
    RC accept(int &clnt_fd) const;
    RC connect(const char *ip, uint16_t port) const;
    RC set_non_blocking() const;
    bool is_non_blocking() const;
    size_t recv_buf_size() const;

private:
    int fd_;
};

#endif