#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "common.h"
#include <functional>
#include <memory>

class Acceptor {

public:
    explicit Acceptor(EventLoop *_loop);
    ~Acceptor();
    DISALLOW_COPY_AND_MOVE(Acceptor);

    RC accept_connection() const;
    void set_new_connection_callback(std::function<void(int)> const &callback);

private:
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    std::function<void(int)> new_connection_callback_;
};

#endif