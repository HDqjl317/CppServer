#ifndef SOCKET_H
#define SOCKET_H

class InetAddress;
class Socket {

public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(InetAddress*);
    void listen();
    void setnonblocking();

    int accept(InetAddress*);
    int getFd();

private:
    int fd;
};

#endif