#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <functional>

class EventLoop;
class Socket;
class InetAddress;
class Channel;

class Acceptor {
public:
    EventLoop *loop;
    Socket *sock;
    InetAddress *addr;
    Channel *acceptChannel;

    Acceptor(EventLoop *_loop);
    ~Acceptor();
    void acceptConnection();
    std::function<void(Socket *)> newConnectionCallback;
    void setNewConnectionCallback(std::function<void(Socket*)>);
};

#endif