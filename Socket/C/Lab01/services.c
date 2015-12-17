#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char ** argv) {
	char nome[128];
	struct servent* service;

	gethostname(nome, sizeof nome);
	printf("My hostname: %s\n", nome);

	if (argc != 3) printf("Wrong argument number.\n");
	else {
		service = getservbyname(argv[1], argv[2]);
		if (!service) printf("Error.\n");
		else printf("Port number: %u\n", ntohs(service->s_port));
	}
}
