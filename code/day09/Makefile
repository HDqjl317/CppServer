server:
	g++ src/util.cpp client.cpp -o client && \
	g++ src/util.cpp server.cpp src/epoll.cpp src/inetAddress.cpp src/socket.cpp src/connection.cpp src/channel.cpp src/eventLoop.cpp src/server.cpp src/acceptor.cpp src/buffer.cpp -o server
clean:
	rm server && rm client