server:
	g++ src/util.cpp src/buffer.cpp src/socket.cpp src/inetAddress.cpp client.cpp -o client && \
	g++ server.cpp \
	-pthread \
	src/util.cpp src/epoll.cpp src/inetAddress.cpp src/socket.cpp src/connection.cpp \
	src/channel.cpp src/eventLoop.cpp src/server.cpp src/acceptor.cpp src/buffer.cpp \
	src/threadPoll.cpp \
	-o server
clean:
	rm server && rm client

threadTest:
	g++ -pthread src/threadPool.cpp threadPo1lTest.cpp -o threadPo1lTest