src=$(wildcard src/*.cpp)

server:
	g++ -std=c++11 -pthread -g \
	$(src) \
	server.cpp \
	-o server
	
client:
	g++ src/util.cpp src/buffer.cpp src/socket.cpp src/inetAddress.cpp client.cpp -o client

th:
	g++ -pthread src/threadPool.cpp threadPoolTest.cpp -o threadPoolTest

test:
	g++ src/util.cpp src/buffer.cpp src/socket.cpp src/inetAddress.cpp src/threadPool.cpp \
	-pthread \
	test.cpp -o test

clean:
	rm server && rm client && rm test