#include "eventLoop.h"
#include "epoll.h"
#include "channel.h"
#include "threadPoll.h"
#include <vector>

EventLoop::EventLoop() : ep(nullptr), quit(false) {
    ep = new Epoll();
    threadPoll = new ThreadPoll();
}

EventLoop::~EventLoop() {
    delete ep;
    delete threadPoll;
}

void EventLoop::loop() {
    while(!quit) {
        std::vector<Channel *> chs;
        chs = ep->poll();
        for(auto it = chs.begin(); it != chs.end(); ++it) {
            (*it)->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel *ch) {
    ep->updateChannel(ch);
}

void EventLoop::addThread(std::function<void()> func) {
    threadPoll->add(func);
}