#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>

#include <sys/types.h>
#include <sys/socket.h>

int main(){
    const char* hostname = 0;
    const char* portname = "daytime";

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_ADDRCONFIG;
    struct addrinfo* res = 0;
    int err = getaddrinfo(hostname, portname, &hints, &res);
    if(err != 0){
        printf("failed to resolve remote socket address (err=%d)",err);
    }
    // create socket
    int fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if (fd==-1) {
        printf("%s",strerror(errno));
    }
    // create data
    char content[256] = "hello from the other side";
    // send the datagram
    if(sendto(fd,content, sizeof(content),0,
    res->ai_addr,res->ai_addrlen) == -1){
        printf("%s",strerror(errno));
    }

    printf("successs.. kinda..\n");
    return 0;
}