#ifndef SOCKET_H
#define SOCKET_H

class inetAddress;
class Socket {

public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(inetAddress);
    void listen();
    void setnonblocking();

    int accept(inetAddress*);
    int getFd();

private:
    int fd;
}

#endif