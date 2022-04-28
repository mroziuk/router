#include <netinet/ip.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

int main(){

    char server_message[256] = "message from server";

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8001);
	server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_sock, (struct sockaddr*) &server_address, sizeof(server_address));

    listen(server_sock, 5);

    int client_socket = accept(server_sock, NULL, NULL);

    send(client_socket, server_message, sizeof(server_message), 0);

    close(server_sock);

    return 0;
}