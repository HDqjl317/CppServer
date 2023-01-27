#ifndef CHANNEL_H
#define CHANNEL_H

#include "common.h"
#include <sys/epoll.h>
#include <functional>

class Channel {
    
public:
    Channel(EventLoop *_loop, int _fd);
    ~Channel();
    DISALLOW_COPY_AND_MOVE(Channel);

    void handle_event() const;
    void enable_read();
    void enable_write();

    int get_fd() const;
    short listen_events() const;
    short ready_events() const;
    bool exist() const;
    void set_exist(bool in = true);
    void enable_et();

    void set_ready_event(short ev);
    void set_read_callback(std::function<void()> const &callback);
    void set_write_callback(std::function<void()> const &callback);

    static const short READ_EVENT;
    static const short WRITE_EVENT;
    static const short ET;

private:
    EventLoop *loop_;
    int fd_;
    short listen_events_;
    short ready_events_;
    bool exist_;
    std::function<void()> read_callback_;
    std::function<void()> write_callback_;
};


#endif
