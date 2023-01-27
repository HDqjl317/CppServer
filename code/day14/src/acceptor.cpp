#include "socket.h"
#include "inet_address.h"
#include "channel.h"
#include "acceptor.h"
#include <utility>
#include <fcntl.h>
#include <assert.h>

Acceptor::Acceptor(EventLoop *loop) {
    socket_ = std::make_unique<Socket>();
    assert(socket_->create() == RC_SUCCESS);
    assert(socket_->bind("127.0.0.1", 1234) == RC_SUCCESS);
    assert(socket_->listen() == RC_SUCCESS);

    channel_ = std::make_unique<Channel>(loop, socket_->fd());
    std::function<void()> cb = std::bind(&Acceptor::accept_connection, this);

    channel_->set_read_callback(cb);
    channel_->enable_read();
}

Acceptor::~Acceptor() {}

RC Acceptor::accept_connection() const{
    int clnt_fd = -1;
    if( socket_->accept(clnt_fd) != RC_SUCCESS ) {
        return RC_ACCEPTOR_ERROR;
    }
    // TODO: remove
    fcntl(clnt_fd, F_SETFL, fcntl(clnt_fd, F_GETFL) | O_NONBLOCK);  // 新接受到的连接设置为非阻塞式
    if (new_connection_callback_) {
        new_connection_callback_(clnt_fd);
    }
    return RC_SUCCESS;
}

void Acceptor::set_new_connection_callback(std::function<void(int)> const &callback) {
    new_connection_callback_ = std::move(callback);
}