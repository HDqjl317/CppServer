#include "channel.h"
#include "event_loop.h"
#include "socket.h"
#include <unistd.h>
#include <utility>

const short Channel::READ_EVENT = 1;
const short Channel::WRITE_EVENT = 2;
const short Channel::ET = 4;

Channel::Channel(EventLoop *loop, int fd) : fd_(fd), loop_(loop), listen_events_(0), ready_events_(0), exist_(false) {}

Channel::~Channel() { loop_->delete_channel(this); }

void Channel::handle_event() const {
    if (ready_events_ & READ_EVENT) {
        read_callback_();
    }
    if (ready_events_ & WRITE_EVENT) {
        write_callback_();
    }
}

void Channel::enable_read() {
    listen_events_ |= READ_EVENT;
    loop_->update_channel(this);
}

void Channel::enable_write() {
    listen_events_ |= WRITE_EVENT;
    loop_->update_channel(this);
}

void Channel::enable_et() {
    listen_events_ |= ET;
    loop_->update_channel(this);
}
int Channel::get_fd() const { return fd_; }

short Channel::listen_events() const { return listen_events_; }
short Channel::ready_events() const { return ready_events_; }

bool Channel::exist() const { return exist_; }

void Channel::set_exist(bool in) { exist_ = in; }

void Channel::set_ready_event(short ev) {
    if (ev & READ_EVENT) {
        ready_events_ |= READ_EVENT;
    }
    if (ev & WRITE_EVENT) {
        ready_events_ |= WRITE_EVENT;
    }
    if (ev & ET) {
        ready_events_ |= ET;
    }
}

void Channel::set_read_callback(std::function<void()> const &callback) { read_callback_ = std::move(callback); }
void Channel::set_write_callback(std::function<void()> const &callback) { write_callback_ = std::move(callback); }