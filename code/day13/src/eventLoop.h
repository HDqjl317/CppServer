#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <functional>
#include "macros.h"

class Epoll;
class Channel;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    DISALLOW_COPY_AND_MOVE(EventLoop);

    void loop();
    void updateChannel(Channel*);

private:
    Epoll *ep{nullptr};
    bool quit{false};
};

#endif