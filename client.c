#include <netinet/ip.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

void print_as_bytes (unsigned char* buff, ssize_t length)
{
	for (ssize_t i = 0; i < length; i++, buff++)
		printf ("%.2x ", *buff);	
}


int main(){
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8001);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int connection = connect(sock, (struct sockaddr *) &server_address, sizeof(server_address));

	if(connection == -1){
		printf("there is an issue\n");
	}
	char server_response[256];
	recv(sock, &server_response, sizeof(server_response), 0);
	printf("response %s", server_response);
	close(sock);

	return EXIT_SUCCESS;
}
