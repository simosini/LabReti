/*iterative server connection-oriented*/

#include <sys/select.h>
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
	//concurrency via select
	fd_set rfds;
	fd_set afds;
	int fd;
	FD_ZERO(&afds);
	FD_SET(mysocket, &afds);
	while(1) {
		bcopy(&afds, &rfds, sizeof(afds));
		result = select(FOPEN_MAX, &rfds, NULL, NULL, NULL);
		if (result < 0) {
			perror("select error\n");
			return -1;
		}
		if (FD_ISSET(mysocket, &rfds)) {
			int sockactive = accept(mysocket, (struct sockaddr *)&indFrom, &indLength);
			if(sockactive < 0){
				perror("accept error\n");
				return -1;
			}
			printf("Connection from client %s %d\n", inet_ntoa(indFrom.sin_addr), ntohs(indFrom.sin_port));
			FD_SET(sockactive, &afds);
		}
		for (fd=0; fd < FOPEN_MAX; fd++) {
			if (fd != mysocket && FD_ISSET(fd, &rfds)) {
				char buffer[100];
				bzero(buffer, 100); //to have all zero bytes
		
				result = recv(fd, buffer, sizeof(buffer), 0);
				//result contains number of bytes received
				if(result < 0){
					perror("error receiving\n");
				} else if (result == 0) {
					//client closed connection
					printf("Client closed connection\n");
					result = close(fd);
					if (result < 0) {
						perror("close error\n");
						return -1;
					}
					FD_CLR(fd, &afds);
				} else {
					struct sockaddr_in cli;
					int cli_dim = sizeof(cli);
					int res = getpeername(fd, (struct sockaddr *)&cli, &cli_dim);
					if (res < 0) {
						perror("getpeername error\n");
					}
					//print message received info
					printf("Received msg %s from %s %d\n", buffer, inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
					result = send(fd, buffer, result, 0);
					if(result < 0) { //result now contains bytes sent successfully
						perror("error sending\n");
					}
				}
			}
		}
	}
}
