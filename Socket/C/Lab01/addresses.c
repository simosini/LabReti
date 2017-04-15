#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
	u_long address;
	if (argc != 3) printf("Wrong argument number.\n");
	else if (strcmp(argv[1],"s")==0) {
		char* hostName = argv[2];
		struct hostent* hostinfo;
		hostinfo = gethostbyname(hostName);
		if (!hostinfo) printf("No host found.\n");
		else {
			struct in_addr castDot;
			castDot.s_addr = *((u_long *) (hostinfo->h_addr_list[0]));
			printf("Indirizzo: %u\n", castDot.s_addr);
		}
	}
	else if (strcmp(argv[1],"d")==0) {
		address = inet_addr(argv[2]);
		if (address == INADDR_NONE) printf("No host found.\n");
		else printf("Indirizzo: %u\n", (unsigned int)address);
	}
	else printf("Invalid argument.\n");
	
	return 0;
}		
