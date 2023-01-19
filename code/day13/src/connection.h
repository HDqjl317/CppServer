#ifndef CONNECTION_H
#define CONNECTION_H

#include <functional>
#include "macros.h"

class EventLoop;
class Socket;
class Channel;
class Buffer;

class Connection {
public:
    Connection(EventLoop *_loop, Socket *_sock);
    ~Connection();
    DISALLOW_COPY_AND_MOVE(Connection);

    enum State {
        Invalid = 1,
        Handshaking,
        Connected,
        Closed,
        Failed,
    };
    void read();
    void write();

    void setDeleteConnectionCallback(std::function<void(Socket *)> const &callback);
    void setOnConnectCallback(std::function<void(Connection *)> const &callback);
    State getState();
    void close();
    void setSendBuffer(const char *str);
    Buffer *getReadBuffer();
    const char *readBuffer();
    Buffer *getSendBuffer();
    const char *sendBuffer();
    void getlineSendBuffer();
    Socket *getSocket();

    void onConnect(std::function<void()> fn);

private:
    EventLoop *loop;
    Socket *sock;
    Channel *channel{nullptr};
    State state{State::Invalid};
    Buffer *read_buffer{nullptr};
    Buffer *send_buffer{nullptr};
    std::function<void(Socket *)> delete_connectioin_callback;

    std::function<void(Connection *)> on_connect_callback;

    void readNonBlocking();
    void writeNonBlocking();
    void readBlocking();
    void writeBlocking();
};

#endif