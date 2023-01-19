#ifndef SERVER_H
#define SERVER_H

#include <map>
#include <vector>
#include <functional>
#include "macros.h"

class EventLoop;
class Socket;
class Acceptor;
class Connection;
class ThreadPool;

class Server {
public:
    explicit Server(EventLoop*);
    ~Server();
    DISALLOW_COPY_AND_MOVE(Server);
    
    void newConnection(Socket *sock);
    void deleteConnection(Socket *sock);
    void onConnect(std::function<void(Connection *)> fn);

private:
    EventLoop *mainReactor;
    Acceptor *acceptor;
    std::map<int, Connection*> connections;
    std::vector<EventLoop*> subReactors;
    ThreadPool *thpool;
    std::function<void(Connection *)> on_connect_callback;
};

#endif