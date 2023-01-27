#ifndef CONNECTION_H
#define CONNECTION_H

#include "common.h"
#include <functional>
#include <memory>

class Connection {

public:
    Connection(EventLoop *_loop, int _fd);
    ~Connection();
    DISALLOW_COPY_AND_MOVE(Connection);

    enum State {
        Invalid = 0,
        Connecting,
        Connected,
        Closed,
    };

    void set_delete_connection(std::function<void(int)> const &fn);
    void set_on_connect(std::function<void(Connection *)> const &fn);
    void set_on_recv(std::function<void(Connection *)> const &fn);
    State state() const;
    Socket *socket() const;
    void set_send_buf(const char *str);
    Buffer *read_buf();
    Buffer *send_buf();

    RC read();
    RC write();
    RC send(std::string msg);

    void close();

    void on_connect(std::function<void()> fn);
    void on_message(std::function<void()> fn);

private:
    void business();
    RC read_non_blocking();
    RC write_non_blocking();
    RC read_blocking();
    RC write_blocking();

private:
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;

    State state_;
    std::unique_ptr<Buffer> read_buf_;
    std::unique_ptr<Buffer> send_buf_;

    std::function<void(int)> delete_connectioin_;
    std::function<void(Connection *)> on_recv_;
};

#endif