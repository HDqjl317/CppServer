#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "common.h"
#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>

class TcpServer {

public:
    
    TcpServer();
    ~TcpServer();
    DISALLOW_COPY_AND_MOVE(TcpServer);
    void start();

    RC new_connection(int fd);
    RC delete_connection(int fd);

    void on_connect(std::function<void(Connection *)> fn);
    void on_recv(std::function<void(Connection *)> fn);

private:
    std::unique_ptr<EventLoop> main_reactor_;
    std::unique_ptr<Acceptor> acceptor_;

    std::unordered_map<int, std::unique_ptr<Connection>> connections_;
    std::vector<std::unique_ptr<EventLoop>> sub_reactors_;

    std::unique_ptr<ThreadPool> thread_pool_;

    std::function<void(Connection *)> on_connect_;
    std::function<void(Connection *)> on_recv_;
};

#endif