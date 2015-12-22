#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

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
	/*sending a message*/
	char buffer[100];
	printf("Insert string to send: \n");
	scanf("%[^\n]", buffer);
	/*need to build server address which in this case is on the same host*/
	struct sockaddr_in serv_add;
	serv_add.sin_family = AF_INET;
	serv_add.sin_addr.s_addr = inet_addr(argv[1]); /*from command line must be dotted format*/
	serv_add.sin_port = htons(atoi(argv[2]));
	result = sendto(mysocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&serv_add, (socklen_t) sizeof(serv_add));
	if(result < 0){
		perror("error sendto\n");
		return -1;
	}
	bzero(buffer, 100); /*to use buffer to receive server answer*/
	struct sockaddr_in indfrom;
	unsigned int indlength = sizeof(indfrom);
	result = recvfrom(mysocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&indfrom, (socklen_t *)&indlength);
	if(result < 0){
		perror("error receiveng\n");
		return -1;
	}
	printf("Received from server: %s address %d, %d\n", buffer, indfrom.sin_addr.s_addr, ntohs(indfrom.sin_port));
	/*closing socket*/
	result = close(mysocket);
	if(result < 0){
		perror("error closing\n");
		return -1;
	}
	
	return 0;
}
