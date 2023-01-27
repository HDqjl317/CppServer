#ifndef POLLER_H
#define POLLER_H

#include "common.h"
#include <vector>
#include <sys/epoll.h>

class Poller {

public:
    DISALLOW_COPY_AND_MOVE(Poller);
    Poller();
    ~Poller();

    RC update_channel(Channel *ch) const;
    RC delete_channel(Channel *ch) const;

    std::vector<Channel *> poll(long timeout = -1) const;

private:
    int fd_;
    struct epoll_event *events_{nullptr};
};

#endif