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

void print_as_bytes (unsigned char* buff, ssize_t length){
	for (ssize_t i = 0; i < length; i++, buff++)
		printf ("%.2x ", *buff);	
}

int main(){
    //create socket address
    const char* hostname = 0; /* wildcard */
    const char* portname = "daytime";
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints)); // clear hints memory
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
    struct addrinfo* res = 0;
    int err = getaddrinfo(hostname, portname, &hints, &res);
    if( err != 0 ){
        printf("failed to resolve local socket address (err=%d)",err);  
    }
    // create socket
    int fd = socket(res->ai_family, res->ai_socktype,res->ai_protocol);
    if(fd == -1){
        printf("%s",strerror(errno));
    }
    /*the server socket must be bound to a local address before it can listen for inbound datagrams*/
    if(bind(fd, res->ai_addr, res->ai_addr) == -1){
        printf("%s",strerror(errno));
    }
    // 
    char buffer[549];
    struct sockaddr_storage src_addr;
    socklen_t src_addr_len = sizeof(src_addr);
    ssize_t count = recvfrom(fd,buffer, sizeof(buffer), 0, (struct sockaddr*)&src_addr, &src_addr_len);
    if (count==-1) {
        printf("%s",strerror(errno));
    } else if (count==sizeof(buffer)) {
        printf("datagram too large for buffer: truncated");
    } else {
        print_as_bytes(buffer,count);
    }


    printf("successs.. kinda..\n");
    return 0;
}