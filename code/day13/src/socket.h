#ifndef SOCKET_H
#define SOCKET_H
#include <arpa/inet.h>
#include "macros.h"

class InetAddress;

class Socket {
public:
    Socket();
    explicit Socket(int);
    ~Socket();
    DISALLOW_COPY_AND_MOVE(Socket);

    void bind(InetAddress*);
    void listen();
    void setnonblocking();
    bool isnonblocking();

    int accept(InetAddress*);
    int getFd();
    void connect(InetAddress*);
    void connect(const char *ip, uint16_t port);

private:
    int fd;
};

#endif