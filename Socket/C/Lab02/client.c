#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
/* IP addresses type is in_addr_t which is a uint_32
explicit translate of addresses with functions in arpa/inet.h
inet_addr convert from dotted to in_addr_t, inet_ntoa from in_addr_t to dotted notation */


int main(int argc, char **argv){
	/*creating a socket*/
	int protocol = getprotobyname("udp") -> p_proto; /*getprotobyname returns a pointer to the struct protoent*/
	int desc = socket(AF_INET, SOCK_DGRAM, protocol);
	
	struct sockaddr_in address; /*to store my local address*/
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(0); /*0 gives the SO the choice of the port*/
	/*bind of the socket*/
	int r = bind(desc, (struct sockaddr *)&address, sizeof(address));
	if (r<0) {
		perror("error bind\n");
		return -1;
	}
	/*in order to know which port was chosen by the SO I use getsockname that put the correct values into the sockaddr_in struct passed*/
	unsigned int length = sizeof(address);
	r = getsockname(desc, (struct sockaddr *)&address, (socklen_t *)&length);
	if (r<0) {
		perror("error getsockname\n");
		return -1;
	}
	printf("Local Address : %s, %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
	/*sending a message to server*/
	/*in order to send a msg i have to know the server address and build it up*/
	/*when I start the server i read the parameters which I pass to the client via CLI*/
	
	struct sockaddr_in serv_addr; /*to store server address and port*/
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);/*must be dotted not*/
	serv_addr.sin_port = htons(atoi(argv[2])); /*from server*/
	unsigned int serv_addr_l = sizeof(serv_addr); 
	char buffer[100];
	/*start sending*/
	while(strcmp(buffer, "stop")){
		printf("Insert string:\n");
		scanf(" %[^\n]", buffer); /*to avoid any special char but \n*/
		r = sendto(desc, buffer, sizeof(buffer), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
		if (r<0) {
			perror("error sending\n");
			return -1;
		}
	/*receiveing answer from server*/
		bzero(buffer, 100); /*to reset the buffer*/
		r = recvfrom(desc, buffer, sizeof(buffer), 0, (struct sockaddr *)&serv_addr, (socklen_t *)&serv_addr_l);
		if (r<0) {
			perror("error received\n");
			return -1;
		}
		if(strcmp(buffer, "stop"))
			printf("Received %s from %s, %d\n", buffer, inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
	}/*end while*/
		/*closing socket*/
	r = close(desc);
	if (r<0) {
		perror("error closing\n");
		return -1;
	}
	printf("Socket closed\n");
	return 0;
}  
