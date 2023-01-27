#include "buffer.h"
#include "channel.h"
#include "socket.h"
#include "util.h"
#include "connection.h"
#include <unistd.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <utility>
#include <sys/types.h>
#include <sys/socket.h>

Connection::Connection(EventLoop *loop, int fd) {
    socket_ = std::make_unique<Socket>();
    socket_->set_fd(fd);
    if (loop != nullptr) {
      channel_ = std::make_unique<Channel>(loop, fd);
      channel_->enable_read();
      channel_->enable_et();
    }
    read_buf_ = std::make_unique<Buffer>();
    send_buf_ = std::make_unique<Buffer>();

    state_ = State::Connected;
}

Connection::~Connection() {}

RC Connection::read() {
    if (state_ != State::Connected) {
      perror("Connection is not connected, can not read");
      return RC_CONNECTION_ERROR;
    }
    assert(state_ == State::Connected && "connection state is disconnected!");
    read_buf_->clear();
    if (socket_->is_non_blocking()) {
      return read_non_blocking();
    } else {
      return read_blocking();
    }
}
RC Connection::write() {
    if (state_ != State::Connected) {
      perror("Connection is not connected, can not write");
      return RC_CONNECTION_ERROR;
    }
    RC rc = RC_UNDEFINED;
    if (socket_->is_non_blocking()) {
      rc = write_non_blocking();
    } else {
      rc = write_blocking();
    }
    send_buf_->clear();
    return rc;
}

RC Connection::read_non_blocking() {
    int sockfd = socket_->fd();
    char buf[1024];  // 这个buf大小无所谓
    while (true) {   // 使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
      memset(buf, 0, sizeof(buf));
      ssize_t bytes_read = ::read(sockfd, buf, sizeof(buf));
      if (bytes_read > 0) {
        read_buf_->append(buf, bytes_read);
      } else if (bytes_read == -1 && errno == EINTR) {  // 程序正常中断、继续读取
        printf("continue reading\n");
        continue;
      } else if (bytes_read == -1 &&
                ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // 非阻塞IO，这个条件表示数据全部读取完毕
        break;
      } else if (bytes_read == 0) {  // EOF，客户端断开连接
        printf("read EOF, client fd %d disconnected\n", sockfd);
        state_ = State::Closed;
        close();
        break;
      } else {
        printf("Other error on client fd %d\n", sockfd);
        state_ = State::Closed;
        close();
        break;
      }
    }
    return RC_SUCCESS;
}
RC Connection::write_non_blocking() {
    int sockfd = socket_->fd();
    char buf[send_buf_->size()];
    memcpy(buf, send_buf_->c_str(), send_buf_->size());
    int data_size = send_buf_->size();
    int data_left = data_size;
    while (data_left > 0) {
      ssize_t bytes_write = ::write(sockfd, buf + data_size - data_left, data_left);
      if (bytes_write == -1 && errno == EINTR) {
        printf("continue writing\n");
        continue;
      }
      if (bytes_write == -1 && errno == EAGAIN) {
        break;
      }
      if (bytes_write == -1) {
        printf("Other error on client fd %d\n", sockfd);
        state_ = State::Closed;
        break;
      }
      data_left -= bytes_write;
    }
    return RC_SUCCESS;
}

RC Connection::read_blocking() {
    int sockfd = socket_->fd();
    // unsigned int rcv_size = 0;
    // socklen_t len = sizeof(rcv_size);
    // getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);
    size_t data_size = socket_->recv_buf_size();
    char buf[1024];
    ssize_t bytes_read = ::read(sockfd, buf, sizeof(buf));
    if (bytes_read > 0) {
      read_buf_->append(buf, bytes_read);
    } else if (bytes_read == 0) {
      printf("read EOF, blocking client fd %d disconnected\n", sockfd);
      state_ = State::Closed;
    } else if (bytes_read == -1) {
      printf("Other error on blocking client fd %d\n", sockfd);
      state_ = State::Closed;
    }
    return RC_SUCCESS;
}

RC Connection::write_blocking() {
    // 没有处理send_buffer_数据大于TCP写缓冲区，的情况，可能会有bug
    int sockfd = socket_->fd();
    ssize_t bytes_write = ::write(sockfd, send_buf_->buf().c_str(), send_buf_->size());
    if (bytes_write == -1) {
      printf("Other error on blocking client fd %d\n", sockfd);
      state_ = State::Closed;
    }
    return RC_SUCCESS;
}

RC Connection::send(std::string msg) {
    set_send_buf(msg.c_str());
    write();
    return RC_SUCCESS;
}

void Connection::business() {
    read();
    on_recv_(this);
}

void Connection::set_delete_connection(std::function<void(int)> const &fn) { delete_connectioin_ = std::move(fn); }

void Connection::set_on_recv(std::function<void(Connection *)> const &fn) {
    on_recv_ = std::move(fn);
    std::function<void()> bus = std::bind(&Connection::business, this);
    channel_->set_read_callback(bus);
}

void Connection::close() { delete_connectioin_(socket_->fd()); }

Connection::State Connection::state() const { return state_; }

Socket *Connection::socket() const { return socket_.get(); }

void Connection::set_send_buf(const char *str) { send_buf_->set_buf(str); }
Buffer *Connection::read_buf() { return read_buf_.get(); }
Buffer *Connection::send_buf() { return send_buf_.get(); }