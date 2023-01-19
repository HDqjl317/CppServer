#include "src/eventLoop.h"
#include "src/server.h"
#include "src/buffer.h"
#include "src/connection.h"
#include "src/socket.h"
#include <iostream>

int main() {
    EventLoop *loop = new EventLoop();
    Server *server = new Server(loop);
    server->onConnect([](Connection *conn) {
        conn->read();
        if (conn->getState() == Connection::State::Closed) {
        conn->close();
        return;
        }
        std::cout << "Message from client " << conn->getSocket()->getFd() << ": " << conn->readBuffer() << std::endl;
        conn->setSendBuffer(conn->readBuffer());
        conn->write();
    });

    loop->loop();
    delete server;
    delete loop;
    return 0;
}