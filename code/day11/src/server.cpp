#include "server.h"
#include "socket.h"
#include "connection.h"
#include "acceptor.h"
#include <functional>
#include <string.h>
#include <unistd.h>

#define READ_BUFFER 1024

Server::Server(EventLoop *_Loop) : loop(_Loop), acceptor(nullptr) {
    acceptor = new Acceptor(loop);
    std::function<void(Socket *)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}

Server::~Server() {
    delete acceptor;
}

void Server::newConnection(Socket *sock){
    if (sock->getFd() != -1) {
        Connection *conn = new Connection(loop, sock);
        std::function<void(int)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
        conn->setDeleteConnectionCallback(cb);
        connections[sock->getFd()] = conn;        
    }
    
}

void Server::deleteConnection(int sockfd){
    if (sockfd != -1) {
        Connection *conn = connections[sockfd];
        connections.erase(sockfd);
        delete conn;        
    }

}