#include "src/connection.h"
#include "src/socket.h"
#include <iostream>

int main() {
  Socket *sock = new Socket();
  sock->connect("127.0.0.1", 1234);

  Connection *conn = new Connection(nullptr, sock);

  while (true) {
    conn->getlineSendBuffer();
    conn->write();
    if (conn->getState() == Connection::State::Closed) {
      conn->close();
      break;
    }
    conn->read();
    std::cout << "Message from server: " << conn->readBuffer() << std::endl;
  }

  delete conn;
  return 0;
}