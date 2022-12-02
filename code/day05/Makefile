server:
	g++ util.cpp client.cpp -o client && \
	g++ util.cpp server.cpp epoll.cpp inetAddress.cpp socket.cpp channel.cpp -o server
clean:
	rm server && rm client