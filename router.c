#include <netinet/ip.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#define BUFLEN 9	//Max length of buffer
#define PORT 54321	//The port on which to send data
#include "test.c"

struct connection{
    unsigned char *address; // bajty adresu + maska
    int distance;
    unsigned char via[5];
};
void die(char *s){
	perror(s);
	exit(1);
}

void print_connections(struct connection cons[], int n){
    for(int i=0;i<n;i++){
        char addr[30], via[30] = "directly";
        fromBytesToSrting(cons[i].address, 5, addr);
        //fromBytesToSrting(cons[i].via, 5, via);
        printf("%s distance %d via %s", addr, cons[i].distance, via);
    }
}
struct udp{
    
};
// int main(){
//     struct connection connections[100];
//     //wczytaj konfiguracje domyslna
//     int n = 2;
//     scanf("%d", &n);
//     char input[100];
//     for(int i=0;i<n;i++){
//         char addr[20];
//         int dist;
//         scanf("%s %d", addr, &dist);
//         //fgets(addr,100,stdin);
//         //printf("%s %d", addr, dist);
//         unsigned char buff[5];
//         fromStringToBytes(addr,buff);
//         connections[i].distance = dist;
//         printf("%c %c %c %c", connections[i].address[0], connections[i].address[1],connections[i].address[2],connections[i].address[3]);
//     }
    /*
    for(int i=0;i<n;i++){

        // create socket
        struct sockaddr_in si_other;
        int s, slen=sizeof(si_other);
        char buf[BUFLEN];
        char message[BUFLEN];

        if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        {
            die("socket");
        }

        memset((char *) &si_other, 0, sizeof(si_other));
        si_other.sin_family = AF_INET;
        si_other.sin_port = htons(PORT);
        char* server = connections[i].address;
        puts(server);
        // TODO send to broadcast address

        if (inet_aton(server , &si_other.sin_addr) == 0) 
        {
            fprintf(stderr, "inet_aton() failed\n");
            exit(1);
        }
        //odczytaj pakiety i przetworz


        //wyslij swoje pakiety

    }
    */

    //print_connections(connections, n);
    // char* res;
    // fromBytesToSrting(connections[0].address, 5, res);
    // printf("%s", res);

//     printf("%c", connections[0].address[0]);

//     return 0;
// }