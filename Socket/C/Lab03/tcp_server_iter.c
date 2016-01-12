/*iterative server connection-oriented*/

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
	//this is to allow only 4 different clients
	int count = 0;
	while(count < 4) {
		printf("waiting for client\n");
		int sockactive = accept(mysocket, (struct sockaddr *)&indFrom, &indLength);
		if(sockactive <0){
			perror("accept error\n");
			return -1;
		}
		printf("Connection from client %d %d\n", indFrom.sin_addr.s_addr, ntohs(indFrom.sin_port));
		count++;
		while(1) {	
			//messages exchanges
			char buffer[100];
			bzero(buffer, 100); //to have all zero bytes
		
			result = recv(sockactive, buffer, sizeof(buffer), 0);
			//result contains number of bytes received
			if(result < 0){
				perror("error receiving\n");
				return -1;
			} else if (result == 0) {
				//client closed connection
				printf("Client closed connection\n");
				break;
			}
			//print message received info
			printf("Received msg %s\n", buffer);
			result = send(sockactive, buffer, result, 0);
			if(result < 0) { //result now contains bytes sent successfully
				perror("error sending\n");
				return -1;
			}
		}
		printf("closing active socket\n");
		//closing socket
		result = close(sockactive);
		if (result < 0){
			perror("error closing\n");
			return -1;
		}
	
	}
	printf("closing passive socket\n");
	//closing socket
	result = close(mysocket);
	if (result < 0){
		perror("error closing\n");
		return -1;
	}
		
	return 0;
	
}
