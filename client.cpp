#include <sys/socket.h>
#include <netinet/in.h>
#include <ctime>
#include <pthread.h>
#include <algorithm>
#include <bright_lib/tcp_socket/client.hpp>

using namespace bright_lib::tcp_socket;
using namespace std;

#define SEND_BUF_SZ 100000lu

int main(int argc, char** argv){
	if (argc != 4){
		printf("Usage: ./client <server_ip> <server_port> <size>\n");
		return -1;
	}

	string server_ip = argv[1];
	uint16_t server_port = stoi(argv[2]);
	uint64_t size = stoi(argv[3]);

	int ret;

	Client client;
	ret = client.connect(server_ip, server_port);
	//client.set_quickack(1);

	client.send(size);

	char *buf = new char[SEND_BUF_SZ];
	for (uint64_t recvd = 0; recvd < size;){
		uint64_t len = min(size - recvd, SEND_BUF_SZ);
		ssize_t ret = client.recv_len(buf, len);
		if (!ret)
			recvd += SEND_BUF_SZ;
		else
			printf("recv_len error\n");
	}
	delete[] buf;

	return 0;
}
