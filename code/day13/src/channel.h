#ifndef CHANNEL_H
#define CHANNEL_H

#include <sys/epoll.h>
#include <functional>
#include "macros.h"

class Socket;
class EventLoop;

class Channel {
public:
    Channel(EventLoop *_loop, int _fd);
    ~Channel();
    DISALLOW_COPY_AND_MOVE(Channel);

    void handleEvent();
    void enableRead();

    int getFd();
    uint32_t getEvents();
    uint32_t getReady();
    bool getInEpoll();
    void setInEpoll(bool _in = true);
    void useET();

    void setReady(uint32_t);
    void setReadCallback(std::function<void()> const &callback);

private:
    EventLoop *loop;
    int fd;
    uint32_t events;
    uint32_t ready;
    bool inEpoll;
    std::function<void()> readCallback;
    std::function<void()> writeCallback;
};


#endif
