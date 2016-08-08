#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#define TRUE 1
/* IP addresses type is in_addr_t which is a uint_32
explicit translate of addresses with functions in arpa/inet.h
inet_addr convert from dotted to in_addr_t, inet_ntoa from in_addr_t to dotted notation(interpreted as a string) */


int main(int argc, char **argv){
	/*creating a socket*/
	int protocol = getprotobyname("udp") -> p_proto; /*getprotobyname returns a pointer to the struct protoent*/
	int desc = socket(AF_INET, SOCK_DGRAM, protocol);
	printf("FD : %d\n",desc);
	struct sockaddr_in address; /*to store local address*/
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY; /*special IP addr(0)that allows the program to work without knowing the ip addr of the machine*/
	address.sin_port = htons(0);
	/*binding the socket*/
	int r = bind(desc, (struct sockaddr *)&address, sizeof(address));
	if (r<0) {
		perror("error bind\n");
		return -1;
	}
	/*verify which port the SO gave to the server process*/
	unsigned int length = sizeof(address);
	r = getsockname(desc, (struct sockaddr *)&address, (socklen_t *)&length);
	if (r<0) {
		perror("error getsockname\n");
		return -1;
	}
	printf("Local Address : %s, %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));/*sin_addr will be 0 because of INADDR_ANY*/
	/*receiveing a message from client*/
	char buffer[100];
	bzero(buffer, 100);
	struct sockaddr_in client_addr; /*to hold client address and port*/
	unsigned int client_addr_l = sizeof(client_addr); /*we need a pointer to this value to call function recvfrom*/
	while(strcmp(buffer, "stop")){
		bzero(buffer, 100);
		/*recvfrom sets the values for client's address in order to be able to answer it we pass pointers to allow that*/
		r = recvfrom(desc, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_l);
		if (r<0) {
			perror("error received\n");
			return -1;
		}
		printf("Received %s from %s, %d\n", buffer, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		/*answering the client*/
		r = sendto(desc, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, client_addr_l);
		if (r<0) {
			perror("error sending\n");
			return -1;
		}
	}
	/*closing socket*/
	r = close(desc);
	if (r<0) {
		perror("error closing\n");
		return -1;
	}
	printf("Socket closed\n");
	return 0;
}  
