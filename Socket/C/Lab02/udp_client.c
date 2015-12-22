#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>

int main(int argc, char **argv){
	int mysocket; //per descrittore socket
	struct protoent *pe;
	struct sockaddr_in addr;
	int result; // per salvare valore di ritorno syscall
	
	pe = getprotobyname("udp");
	if(!pe){
		perror("error protocol\n");
		return -1;
	}
	int protocol = pe -> p_proto;
	mysocket = socket(AF_INET, SOCK_DGRAM, protocol);
	if(mysocket < 0){
		perror("error socket\n");
		return -1;
	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(0);
	result = bind(mysocket, (struct sockaddr *) &addr, (socklen_t )sizeof(addr));
	if (result < 0){
		perror("error bind\n");
		return -1;
	}
	
	struct sockaddr_in local_add;
	unsigned int add_length = sizeof(local_add);
	result = getsockname(mysocket, (struct sockaddr *) &local_add, (socklen_t *)&add_length);
	if(result < 0){
		perror("error getsockname\n");
		return -1;
	}
	printf("Local Address: %d, %d\n", local_add.sin_addr.s_addr, ntohs(local_add.sin_port));
	printf("Local Address: %d, %d\n", local_add.sin_addr.s_addr, local_add.sin_port);	
	 	
	return 0;
}
