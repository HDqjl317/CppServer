#ifndef SERVER_H
#define SERVER_H

#include <map>

class EventLoop;
class Socket;
class Acceptor;
class Connection;

class Server {
public:
    Server(EventLoop*);
    ~Server();
    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
    void deleteConnection(Socket *sock);

private:
    EventLoop *loop;
    Acceptor *acceptor;
    std::map<int, Connection*> connections;
};

#endif