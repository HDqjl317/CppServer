#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <functional>

class Epoll;
class Channel;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void updateChannel(Channel*);

private:
    Epoll *ep;
    bool quit;
};

#endif