#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "common.h"
#include <memory>

class EventLoop {

public:
    EventLoop();
    ~EventLoop();
    DISALLOW_COPY_AND_MOVE(EventLoop);

    void loop() const;
    void update_channel(Channel*) const;
    void delete_channel(Channel*) const;

private:
    std::unique_ptr<Poller> poller_;
};

#endif