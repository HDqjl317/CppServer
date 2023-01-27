#include "event_loop.h"
#include "poller.h"
#include "channel.h"
#include <vector>

EventLoop::EventLoop() { poller_ = std::make_unique<Poller>(); }

EventLoop::~EventLoop() {}

void EventLoop::loop() const {
  while (true) {
    for (Channel *active_ch : poller_->poll()) {
      active_ch->handle_event();
    }
  }
}

void EventLoop::update_channel(Channel *ch) const { poller_->update_channel(ch); }

void EventLoop::delete_channel(Channel *ch) const { poller_->delete_channel(ch); }