#include <unistd.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <utility>
#include "buffer.h"
#include "channel.h"
#include "socket.h"
#include "util.h"
#include "connection.h"
#include <sys/types.h>
#include <sys/socket.h>

Connection::Connection(EventLoop *_loop, Socket *_sock) : loop(_loop), sock(_sock), channel(nullptr) {
    if (loop != nullptr) {
        channel = new Channel(loop, sock->getFd());
        channel->enableRead();
        channel->useET();
    }
    read_buffer = new Buffer();
    send_buffer = new Buffer();
    state = State::Connected;
}

Connection::~Connection(){
  if (loop != nullptr) {
    delete channel;
  }
  delete sock;
  delete read_buffer;
  delete send_buffer;
}
void Connection::read() {
    ASSERT(state == State::Connected, "connection state is disconnected!");
    read_buffer->clear();
    if (sock->isnonblocking()) {
        readNonBlocking();
    } else {
        readBlocking();
    }
}
void Connection::write() {
  ASSERT(state == State::Connected, "connection state is disconnected!");
  if (sock->isnonblocking()) {
    writeNonBlocking();
  } else {
    writeBlocking();
  }
  send_buffer->clear();
}

void Connection::readNonBlocking() {
  int sockfd = sock->getFd();
  char buf[1024];  // 这个buf大小无所谓
  while (true) {   // 使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
    memset(buf, 0, sizeof(buf));
    ssize_t bytes_read = ::read(sockfd, buf, sizeof(buf));
    if (bytes_read > 0) {
      read_buffer->append(buf, bytes_read);
    } else if (bytes_read == -1 && errno == EINTR) {  // 程序正常中断、继续读取
      printf("continue reading\n");
      continue;
    } else if (bytes_read == -1 &&
               ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // 非阻塞IO，这个条件表示数据全部读取完毕
      break;
    } else if (bytes_read == 0) {  // EOF，客户端断开连接
      printf("read EOF, client fd %d disconnected\n", sockfd);
      state = State::Closed;
      break;
    } else {
      printf("Other error on client fd %d\n", sockfd);
      state = State::Closed;
      break;
    }
  }
}
void Connection::writeNonBlocking() {
  int sockfd = sock->getFd();
  char buf[send_buffer->size()];
  memcpy(buf, send_buffer->c_str(), send_buffer->size());
  int data_size = send_buffer->size();
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
      state = State::Closed;
      break;
    }
    data_left -= bytes_write;
  }
}

/**
 * @brief Never used by server, only for client
 *
 */
void Connection::readBlocking() {
  int sockfd = sock->getFd();
  unsigned int rcv_size = 0;
  socklen_t len = sizeof(rcv_size);
  getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);
  char buf[rcv_size];
  ssize_t bytes_read = ::read(sockfd, buf, sizeof(buf));
  if (bytes_read > 0) {
    read_buffer->append(buf, bytes_read);
  } else if (bytes_read == 0) {
    printf("read EOF, blocking client fd %d disconnected\n", sockfd);
    state = State::Closed;
  } else if (bytes_read == -1) {
    printf("Other error on blocking client fd %d\n", sockfd);
    state = State::Closed;
  }
}

/**
 * @brief Never used by server, only for client
 *
 */
void Connection::writeBlocking() {
  // 没有处理send_buffer_数据大于TCP写缓冲区，的情况，可能会有bug
  int sockfd = sock->getFd();
  ssize_t bytes_write = ::write(sockfd, send_buffer->c_str(), send_buffer->size());
  if (bytes_write == -1) {
    printf("Other error on blocking client fd %d\n", sockfd);
    state = State::Closed;
  }
}

void Connection::close() { delete_connectioin_callback(sock); }

Connection::State Connection::getState() { return state; }
void Connection::setSendBuffer(const char *str) { send_buffer->setBuf(str); }
Buffer *Connection::getReadBuffer() { return read_buffer; }
const char *Connection::readBuffer() { return read_buffer->c_str(); }
Buffer *Connection::getSendBuffer() { return send_buffer; }
const char *Connection::sendBuffer() { return send_buffer->c_str(); }

void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> const &callback) {
  delete_connectioin_callback = callback;
}
void Connection::setOnConnectCallback(std::function<void(Connection *)> const &callback) {
  on_connect_callback = callback;
  channel->setReadCallback([this]() { on_connect_callback(this); });
}

void Connection::getlineSendBuffer() { send_buffer->getline(); }

Socket *Connection::getSocket() { return sock; }