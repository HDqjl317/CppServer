#include "poller.h"
#include "channel.h"
#include "socket.h"
#include "util.h"
#include <unistd.h>
#include <cstring>

#define MAX_EVENTS 1000

Poller::Poller() {
    fd_ = epoll_create1(0);
    errif(fd_ == -1, "epoll create error");
    events_ = new epoll_event[MAX_EVENTS];
    memset(events_, 0, sizeof(*events_) * MAX_EVENTS);
}

Poller::~Poller() {
    if (fd_ != -1) {
        close(fd_);
    }
    delete[] events_;
}

std::vector<Channel *> Poller::poll(long timeout) const{
    std::vector<Channel *> active_channels;
    int nfds = epoll_wait(fd_, events_, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    for (int i = 0; i < nfds; ++i) {
        Channel *ch = (Channel *)events_[i].data.ptr;
        ch->set_ready_event(events_[i].events);
        active_channels.push_back(ch);
    }
    return active_channels;
}

RC Poller::update_channel(Channel *ch) const{
    int fd = ch->get_fd();
    struct epoll_event ev {};
    ev.data.ptr = ch;
    ev.events = ch->listen_events();
    if (!ch->exist()) {
        errif(epoll_ctl(fd_, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
        ch->set_exist();
    } else {
        errif(epoll_ctl(fd_, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
    }
}

RC Poller::delete_channel(Channel *ch) const{
    int sockfd = ch->get_fd();
    errif(epoll_ctl(fd_, EPOLL_CTL_DEL, sockfd, nullptr) == -1, "epoll delete error");
    ch->set_exist(false);
}
