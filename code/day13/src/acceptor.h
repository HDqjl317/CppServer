#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <functional>
#include "macros.h"

class EventLoop;
class Socket;
class InetAddress;
class Channel;

class Acceptor {
private:
    EventLoop *loop;
    Socket *sock;
    Channel *acceptChannel;
    std::function<void(Socket*)> newConnectionCallback;
public:
    explicit Acceptor(EventLoop *_loop);
    ~Acceptor();
    DISALLOW_COPY_AND_MOVE(Acceptor);

    void acceptConnection();
    void setNewConnectionCallback(std::function<void(Socket*)> const &callback);
};

#endif