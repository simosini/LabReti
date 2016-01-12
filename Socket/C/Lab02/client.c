#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>

int main (int argc, char **argv) {
	int domain = AF_INET;
	int type = SOCK_DGRAM;
	int protocol = 0;

	int mysock;
	struct protoent* pe;
	struct sockaddr_in address;
	int r;

	pe = getprotobyname("udp");
	if (!pe) {
		perror("Getprotobyname error\n");
		return -1;
	}

	protocol = pe->p_proto;
	mysock = socket(domain, type, protocol);
	if (mysock < 0) {
		perror("Socket error\n");
		return -1;
	}
	address.sin_family = domain;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(0);
			
	r = bind(mysock, (struct sockaddr*)&address, sizeof(address));
	if (r < 0) {
		perror("Bind error\n");
		return -1;
	}

	struct sockaddr_in local_address;
	unsigned int address_length = sizeof(local_address);
	r = getsockname(mysock, (struct sockaddr *)&local_address, (socklen_t *) &address_length);
	if (r < 0) {
		perror("Getsockname error\n");
		return -1;
	}

	printf("Local address: %d:%d\n", local_address.sin_addr.s_addr, ntohs(local_address.sin_port));
}
