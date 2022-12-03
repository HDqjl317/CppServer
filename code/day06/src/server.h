#ifndef SERVER_H
#define SERVER_H

class EventLoop;
class Socket;

class Server {
public:
    Server();
    ~Server();
    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
private:
    EventLoop *loop;
}