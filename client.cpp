#include <sys/socket.h>
#include <netinet/in.h>
#include <ctime>
#include <pthread.h>
#include <algorithm>
#include <unistd.h>
#include <cstdlib>
#include <thread>
#include <bright_lib/tcp_socket/client.hpp>

using namespace bright_lib::tcp_socket;
using namespace std;

void print_usage(void){
	printf("Usage: ./client [<options>...]\n");
	printf("Options:\n");
	printf("        -c      specify the destination ip\n");
	printf("        -p      specify the destination port\n");
	printf("        -s      specify request size\n");
	printf("        -P      specify the number of parallel requests\n");
	printf("        -b      specify the msg buffer size\n");
}

void client_fn(string server_ip, uint16_t server_port, uint64_t size, char* buf, int buf_size){
	int ret;

	Client client;
	ret = client.connect(server_ip, server_port);
	//client.set_quickack(1);

	client.send(size);

	for (uint64_t recvd = 0; recvd < size;){
		uint64_t len = min(size - recvd, (uint64_t)buf_size);
		ssize_t ret = client.recv_len(buf, len);
		if (!ret)
			recvd += buf_size;
		else
			printf("recv_len error\n");
	}
}

int main(int argc, char** argv){
	string server_ip = "";
	uint16_t server_port = 0;
	uint64_t size = 0;
	int parallel = 1;
	int buf_size = 100000;

	// parse args
	for (int opt; (opt = getopt(argc, argv, "c:p:s:P:b:h")) != -1; ){
		switch (opt){
			case 'c':
				server_ip = optarg;
				break;
			case 'p':
				server_port = stoi(optarg);
				break;
			case 's':
				size = stoi(optarg);
				break;
			case 'P':
				parallel = stoi(optarg);
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
	if (server_ip == "" || server_port == 0 || size == 0){
		print_usage();
		return 0;
	}

	// init buf
	vector<char*> buf;
	for (int i = 0; i < parallel; i++)
		buf.push_back(new char[buf_size]);

	// run
	vector<thread> threads;
	for (int i = 0; i < parallel; i++)
		threads.push_back(thread(client_fn, server_ip, server_port, size, buf[i], buf_size));

	// wait for finish
	for (int i = 0; i < parallel; i++)
		threads[i].join();

	// free buf
	for (int i = 0; i < parallel; i++)
		delete[] buf[i];
	return 0;
}
