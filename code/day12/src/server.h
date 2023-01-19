#ifndef SERVER_H
#define SERVER_H

#include <map>
#include <vector>

class EventLoop;
class Socket;
class Acceptor;
class Connection;
class ThreadPool;

class Server {
public:
    Server(EventLoop*);
    ~Server();
    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
    void deleteConnection(int sockfd);

private:
    EventLoop *mainReactor;
    Acceptor *acceptor;
    std::map<int, Connection*> connections;
    std::vector<EventLoop*> subReactors;
    ThreadPool *thpool;
};

#endif