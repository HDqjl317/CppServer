#ifndef CHANNEL_H
#define CHANNEL_H

#include <sys/epoll.h>
#include <functional>

class EventLoop;

class Channel {
public:
    Channel(EventLoop *_ep, int _fd);
    ~Channel();

    void enableReading();
    int getFd();

    uint32_t getEvents();
    uint32_t getRevents();
    //void setEvents(uint32_t);
    void setRevents(uint32_t);
    bool getInEpoll();
    void setInEpoll();

    void handleEvent();
    void setCallback(std::function<void()>);

private:
    EventLoop *loop;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
    std::function<void()> callback;
};

#endif
