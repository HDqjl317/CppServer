#include "src/pine.h"
#include <iostream>

int main() {
    Socket *sock = new Socket();
    sock->create();
    sock->connect("127.0.0.1", 1234);

    Connection *conn = new Connection(nullptr, sock->fd());

    while (true) {
      std::string input;
      std::getline(std::cin, input);
      conn->set_send_buf(input.c_str());
      conn->write();
      if (conn->state() == Connection::State::Closed) {
        conn->close();
        break;
      }
      conn->read();
      std::cout << "Message from server: " << conn->read_buf()->c_str() << std::endl;
    }

    delete conn;
    delete sock;
    return 0;
}
