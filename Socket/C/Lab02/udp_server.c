/*la parte di creazione e bind è la stessa del client per udp*/

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
	//host to network short(htons) to convert host endian to big endian 		which is the network format
	addr.sin_port = htons(0); 
	result = bind(mysocket, (struct sockaddr *) &addr, (socklen_t )sizeof(addr));
	if (result < 0){
		perror("error bind\n");
		return -1;
	}
	
	struct sockaddr_in local_add;
	unsigned int add_length = sizeof(local_add);
	result = getsockname(mysocket, (struct sockaddr *) &local_add, (socklen_t *)&add_length); // return 0 if ok
	if(result){
		perror("error getsockname\n");
		return -1;
	}
	printf("Local Address: %d, %d\n", local_add.sin_addr.s_addr, ntohs(local_add.sin_port));
	/*messages exchanges*/
	char buffer[100];
	bzero(buffer, 100); /*to have all zero bytes*/
	struct sockaddr_in add_from;
	 unsigned int add_l = sizeof(add_from);
	result = recvfrom(mysocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&add_from, (socklen_t *)&add_l);
	/*result contains number of bytes received*/
	if(result < 0){
		perror("error receiveing");
		return -1;
	}
	/*print message received info*/
	printf("Received msg %s from %s, %d\n", buffer,inet_ntoa(add_from.sin_addr), ntohs(add_from.sin_port));
	result = sendto(mysocket, buffer, result, 0, (struct sockaddr*)&add_from, (socklen_t)add_l);
	if(result < 0) { /*result now contains bytes sent successfully*/
		perror("error sendto\n");
		return -1;
	}
	/*closing socket*/
	result = close(mysocket);
	if (result <0){
		perror("error closing\n");
		return -1;
	}
		
	return 0;
	
}
