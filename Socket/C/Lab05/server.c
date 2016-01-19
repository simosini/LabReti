#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
//multithread concurrency
#include <wait.h>
#include <signal.h>

int echo(int sockactive) {
	char buffer[100];
	bzero(buffer, 100); //to have all zero bytes
	int result;
	
	do {
		result = recv(sockactive, buffer, sizeof(buffer), 0);
		struct sockaddr_in cli;
		int cli_dim = sizeof(cli);
		int res = getpeername(sockactive, (struct sockaddr *)&cli, &cli_dim);
		if (res < 0) {
			perror("getpeername error\n");
		}
		//result contains number of bytes received
		if(result < 0){
			perror("error receiving\n");
		} else if (result == 0) {
			//client closed connection
			printf("Client %s %d closed connection\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
			result = close(sockactive);
			if (result < 0) {
				perror("close error\n");
			}
		} else {
			//print message received info
			printf("Received msg %s from %s %d\n", buffer, inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
			result = send(sockactive, buffer, result, 0);
			if(result < 0) { //result now contains bytes sent successfully
				perror("error sending\n");
			}
		}
	} while(result > 0);
	return result;
}

void sig_handler() {
	int status;
	while(wait3(&status, WNOHANG, NULL) > 0);
	//reconnecting handler for future signals
	signal(SIGCHLD, sig_handler);
}

int main(int argc, char **argv){
	int mysocket; //per descrittore socket
	struct protoent *pe;
	struct sockaddr_in indFrom;
	socklen_t indLength = sizeof(indFrom);
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
	
	indFrom.sin_family = AF_INET;
	indFrom.sin_addr.s_addr = INADDR_ANY;
	//host to network short(htons) to convert host endian to big endian 		which is the network format
	indFrom.sin_port = htons(0); 
	result = bind(mysocket, (struct sockaddr *) &indFrom, (socklen_t )sizeof(indFrom));
	if (result < 0){
		perror("error bind\n");
		return -1;
	}
	//printing local port
	struct sockaddr_in local_add;
	unsigned int add_length = sizeof(local_add);
	result = getsockname(mysocket, (struct sockaddr *) &local_add, (socklen_t *)&add_length); // return 0 if ok
	if(result){
		perror("error getsockname\n");
		return -1;
	}
	printf("Local Address: %d, %d\n", local_add.sin_addr.s_addr, ntohs(local_add.sin_port));
	//connection
	result = listen(mysocket, 20);
	if(result < 0){
		perror("listen error\n");
		return -1; 
	}
	//connecting our handler for the first time
	signal(SIGCHLD, sig_handler);
	struct sockaddr_in indfrom;
	int indlength = sizeof(indfrom);
	while(1) {
		int sockactive = accept(mysocket, (struct sockaddr *)&indFrom, &indLength);
		if(sockactive < 0){
			perror("accept error\n");
			return -1;
		}
		printf("Connection from client %s %d\n", inet_ntoa(indFrom.sin_addr), ntohs(indFrom.sin_port));

		switch(fork()) {
			case 0: 
				close(mysocket);
				exit(echo(sockactive));
			case -1:
				perror("accept error\n");
				return -1;
			default: 
				close(sockactive);
		}
	}
	result = close(mysocket);
	if (result < 0) {
		perror("close error\n");
		return -1;
	}
}
