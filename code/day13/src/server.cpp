#include "server.h"
#include "socket.h"
#include "connection.h"
#include "acceptor.h"
#include "threadPool.h"
#include "eventLoop.h"
#include "util.h"
#include <functional>
#include <string.h>
#include <unistd.h>

#define READ_BUFFER 1024

Server::Server(EventLoop *_Loop) : mainReactor(_Loop), acceptor(nullptr), thpool(nullptr) {
    acceptor = new Acceptor(mainReactor);
    std::function<void(Socket *)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);

    int size = static_cast<int>(std::thread::hardware_concurrency());
    thpool = new ThreadPool(size);
    for(int i = 0; i < size; ++i) {
        subReactors.push_back(new EventLoop());
    }

    for(int i = 0; i < size; ++i) {
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, subReactors[i]);
        thpool->add(sub_loop);
    }
}

Server::~Server() {
    delete acceptor;
    delete thpool;
}

void Server::newConnection(Socket *sock){
    errif(sock->getFd() == -1, "new connection error");
    uint64_t random = sock->getFd() % subReactors.size();
    Connection *conn = new Connection(subReactors[random], sock);
    std::function<void(Socket *)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    conn->setOnConnectCallback(on_connect_callback);
    connections[sock->getFd()] = conn;
}
void Server::deleteConnection(Socket *sock){
    int sockfd = sock->getFd();
    auto it = connections.find(sockfd);
    if (it != connections.end()) {
        Connection *conn = connections[sockfd];
        connections.erase(sockfd);
        delete conn;
        conn = nullptr;
    }
}

void Server::onConnect(std::function<void(Connection *)> fn) { on_connect_callback = std::move(fn); }