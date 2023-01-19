#ifndef INETADDRESS_H
#define INETADDRESS_H

#include <arpa/inet.h>
#include "macros.h"

class InetAddress {
private:
    struct sockaddr_in addr;
    socklen_t addr_len;

public:
    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    DISALLOW_COPY_AND_MOVE(InetAddress);

    ~InetAddress();
    void setInetAddr(sockaddr_in _addr, socklen_t _addr_len);
    sockaddr_in getAddr();
    socklen_t getAddr_len();
    const char *getIp();
    uint16_t getPort();

private:
    struct sockaddr_in addr_ {};
};

#endif