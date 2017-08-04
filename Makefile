CFLAGS = -std=gnu++11 -O3

all: client server

client: client.cpp
	g++ client.cpp -o client $(CFLAGS) -lpthread -ltcp_socket

server: server.cpp
	g++ server.cpp -o server $(CFLAGS) -lpthread -ltcp_socket
