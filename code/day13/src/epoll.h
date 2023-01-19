#ifndef EPOLL_H
#define EPOLL_H

#include <vector>
#include "macros.h"

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif

class Channel;

class Epoll {
public:
    Epoll();
    ~Epoll();
    DISALLOW_COPY_AND_MOVE(Epoll);

    void addFd(int fd, uint32_t op);
    void updateChannel(Channel*);
    void deleteChannel(Channel*);
    std::vector<Channel *> poll(int timeout = -1);

private:
    int epfd{1};
    struct epoll_event *events{nullptr};

};

#endif