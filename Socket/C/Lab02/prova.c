#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
/*to request DNS service same as shell command nslookup*/
int main(){
	char buffer[100];
	printf("Insert name:\n");
	scanf(" %[^\n]",buffer);
	struct in_addr address;
	while(strcmp(buffer, "stop")){
		address.s_addr = *(in_addr_t *) gethostbyname(buffer)->h_addr_list[0];
		printf("Address: %u\n", address.s_addr);/*as a u_int32*/
		printf("Address: %s\n", inet_ntoa(address));/*dotted notation*/
		bzero(buffer, 100);
		printf("Insert name:\n");
		scanf(" %[^\n]",buffer);
	}
	return 0;
}
