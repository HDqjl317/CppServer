#ifndef CONNECTION_H
#define CONNECTION_H

#include <functional>

class EventLoop;
class Socket;
class Channel;
class Buffer;

class Connection {
public:
    Connection(EventLoop *_loop, Socket *_sock);
    ~Connection();
    void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void(int)>);
    void send(int sockfd);
private:
    EventLoop *loop;
    Socket *sock;
    Channel *channel;
    std::function<void(int)> deleteConnectionCallback;
    std::string *inBuffer;
    Buffer *readBuffer;
};

#endif