#ifndef SERVER_H
#define SERVER_H

class EventLoop;
class Socket;
class Acceptor;

class Server {
public:
    Server(EventLoop*);
    ~Server();
    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
private:
    EventLoop *loop;
    Acceptor *acceptor;
};

#endif