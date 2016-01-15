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
	
	pe = getprotobyname("tcp");
	if(!pe){
		perror("error protocol\n");
		return -1;
	}
	int protocol = pe -> p_proto;
	mysocket = socket(AF_INET, SOCK_STREAM, protocol);
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
	
	/*connecting to server*/
	struct sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	struct hostent *host = gethostbyname(argv[1]);
	servAddr.sin_addr.s_addr = *(u_long *)host->h_addr_list[0];
	servAddr.sin_port = htons(atoi(argv[2]));
	result = connect(mysocket, (struct sockaddr *)&servAddr, sizeof(servAddr));
	if(result < 0) {
		perror("Connection error\n");
		return -1;
	}
	
	struct sockaddr_in local_add;
	unsigned int add_length = sizeof(local_add);
	result = getsockname(mysocket, (struct sockaddr *) &local_add, (socklen_t *)&add_length);
	if(result < 0){
		perror("error getsockname\n");
		return -1;
	}
	printf("Local Port: %d\n", ntohs(local_add.sin_port));
	
	while(1) {
	//sending a message
	char buffer[100];
	printf("Insert string to send: \n");
	scanf(" %[^\n]", buffer);
	if (strcmp(buffer, ".") == 0) break;
	//sending data to server
	result = send(mysocket, buffer, sizeof(buffer),0);
	if(result < 0){
		perror("Error sending\n");
		return -1;
	}

	bzero(buffer, 100); //to use buffer to receive server answer
	result = recv(mysocket, buffer, sizeof(buffer), 0);
	if(result < 0){
		perror("error receiving\n");
		return -1;
	}else if (result==0) {
		perror("server closed connection\n");
		return -1;
	}
	printf("Received %s\n", buffer);
	}
	//closing socket
	result = close(mysocket);
	if(result < 0){
		perror("error closing\n");
		return -1;
	}

	return 0;
}
