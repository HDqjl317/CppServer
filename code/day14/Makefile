src=$(wildcard src/*.cpp)

server:
	g++ -std=c++14 -pthread -g \
	$(src) \
	echo_server.cpp \
	-o echo_server
	
client:
	g++ -std=c++14 -pthread src/acceptor.cpp src/buffer.cpp src/util.cpp  \
	src/channel.cpp src/connection.cpp src/event_loop.cpp \
	src/inet_address.cpp src/poller.cpp src/socket.cpp \
	src/tcp_server.cpp src/thread_pool.cpp echo_client.cpp -o echo_client

clean:
	rm server && rm client && rm test