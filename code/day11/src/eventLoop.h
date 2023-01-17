#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <functional>

class Epoll;
class Channel;
class ThreadPool;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void updateChannel(Channel*);
    void addThread(std::function<void()>);

private:
    Epoll *ep;
    bool quit;
    ThreadPool *threadPool;
};

#endif