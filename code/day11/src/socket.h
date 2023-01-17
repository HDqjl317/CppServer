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
    void connect(InetAddress*);

private:
    int fd;
};

#endif