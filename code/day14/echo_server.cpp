#include <iostream>
#include "src/pine.h"

int main() {
    TcpServer *server = new TcpServer();

    Signal::signal(SIGINT, [&] {
      delete server;
      std::cout << "\nServer exit!" << std::endl;
      exit(0);
    });

    server->on_connect([](Connection *conn) { std::cout << "New connection fd: " << conn->socket()->fd() << std::endl; });

    server->on_recv([](Connection *conn) {
      std::cout << "Message from client " << conn->read_buf()->c_str() << std::endl;
      conn->send(conn->read_buf()->c_str());
    });

    server->start();

    delete server;
    return 0;
}
