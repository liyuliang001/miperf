#include <cstdio>
#include <string>
#include <algorithm>
#include <netinet/tcp.h>
#include <bright_lib/tcp_socket/server.hpp>

using namespace std;
using namespace bright_lib::tcp_socket;

class ReceiverHandler: public ServerHandler{
public:
#define BUF_SZ 1000000lu
	char buf[BUF_SZ];
	virtual int execute(){
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
			uint64_t to_send = min(size - sent, BUF_SZ);
			ssize_t ret = send_raw(buf, to_send);
			sent += ret;
		}
		return 0;
	}
};

int main(int argc, char** argv){
	if (argc != 2){
		printf("Usage: ./server <port>\n");
		return 0;
	}

	uint16_t port = stoi(argv[1]);
	Server<ReceiverHandler> s;
	s.listen(port, 1000, false);
	return 0;
}
