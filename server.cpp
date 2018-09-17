#include <cstdio>
#include <string>
#include <algorithm>
#include <netinet/tcp.h>
#include <bright_lib/tcp_socket/server.hpp>

using namespace std;
using namespace bright_lib::tcp_socket;

int buf_size = 100000;

void print_usage(void){
	printf("Usage: ./server [<options>...]\n");
	printf("Options:\n");
	printf("        -p      specify the destination port\n");
	printf("        -b      specify the msg buffer size\n");
}

class ReceiverHandler: public ServerHandler{
public:
	char *buf;
	virtual int execute(){
		int buf_sz = buf_size;
		buf = new char[buf_sz];
		#if 0
		int tmp=100;
		socklen_t optlen=100;
		getsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &tmp, &optlen);
		printf("nodelay = %d\n", tmp);
		set_nodelay(1);
		getsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &tmp, &optlen);
		printf("nodelay = %d\n", tmp);
		#endif
		uint64_t size;
		// get the size to send
		recv(size);
		// send 
		for (uint64_t sent = 0; sent < size;){
			uint64_t to_send = min(size - sent, (uint64_t)buf_sz);
			ssize_t ret = send_raw(buf, to_send);
			sent += ret;
		}
		close();
		delete[] buf;
		return 0;
	}
};

int main(int argc, char** argv){
	uint16_t port = 0;
	// parse args
	for (int opt; (opt = getopt(argc, argv, "p:b:h")) != -1; ){
		switch (opt){
			case 'p':
				port = stoi(optarg);
				break;
			case 'b':
				buf_size = stoi(optarg);
				break;
			case 'h':
			default:
				print_usage();
				return 0;
		}
	}
	if (port == 0){
		print_usage();
		return 0;
	}

	Server<ReceiverHandler> s;
	s.listen(port, 1000, false);
	return 0;
}
