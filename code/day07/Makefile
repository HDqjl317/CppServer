server:
	g++ src/util.cpp client.cpp -o client && \
	g++ src/util.cpp server.cpp src/epoll.cpp src/inetAddress.cpp src/socket.cpp src/channel.cpp src/eventLoop.cpp src/server.cpp src/acceptor.cpp -o server
clean:
	rm server && rm client