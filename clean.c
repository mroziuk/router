#include<stdio.h>	//printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include <time.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUFLEN 9	//Max length of buffer
#define PORT 54321	//The port on which to send data
#define DELAY 5.0
void print_as_bytes (unsigned char* buff, ssize_t length){
	for (ssize_t i = 0; i < length; i++, buff++)
		printf ("%.2x ", *buff);	
}

void fromBytesToSrting(unsigned char* buff, ssize_t length, char res[]){
    sprintf(res, "%d.%d.%d.%d/%d", buff[0],buff[1],buff[2],buff[3],buff[4]);
}
void fromStringToBytes(char str[], unsigned char buff[] ){
    //int buff[5];
    int index = 0;
    char* rest = NULL;
    unsigned char* token;

    for(token = strtok_r(str, "./", &rest);
        token != NULL;
        token = strtok_r(NULL, "./", &rest)){
            //printf("%s", token);
            buff[index] = (unsigned char)atoi(token);
            index ++;
    }
}
struct connection{
    unsigned char address[5]; // bajty adresu + maska
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
        printf("%s distance %d via %s\n", addr, cons[i].distance, via);
    }
}

void getUserInput(struct connection c[], int *n){
// read connections from stdandard input
    char buff[50];
    //scanf("%d", &n);
    char nbuff[10];
    fgets(nbuff, 10, stdin);
    *n = atoi(nbuff);

    for (int i = 0; i < *n; i++){
        fgets(buff, 50, stdin);
        //split into 3 words
        char* rest = NULL;
        char* token;
        int index = 0;
        char *addr;
        int d;
        for(token = strtok_r(buff, " ", &rest);
        token != NULL;
        token = strtok_r(NULL, " ", &rest)){
            //printf("%s", token);
            if(index == 0){
                addr = token;
            }else if(index == 2){
                d = atoi(token);
            }
            index ++;
        }
        unsigned char bytes[5];
        fromStringToBytes(addr,bytes);
        memcpy(c[i].address, bytes,5);
        c[i].distance = d;
    }
}


int main(int argc, char const *argv[]){
    //create array of connections
    int n;
    struct connection c[100];
    // get initial configuration
    getUserInput(c, &n);


    //creating data for server side / listening
    struct sockaddr_in si_me, si_other;
	int s, i, slen = sizeof(si_other) , recv_len;
	char buf[BUFLEN];
	//create a UDP socket
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
		die("socket");
	}
	// zero out the structure
	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	//bind socket to port
	if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1){
		die("bind");
	}
    // end setting up server

    time_t start, end;        
    while(1){
        // time(&start);
        // do{
        //     time(&end);
            //recieve packets here
            //try to receive some data, this is a blocking call
            if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1){
                die("recvfrom()");
            }
            //print details of the client/peer and the data received
            printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
            printf("Data: %s\n" , buf);
            //now reply the client with the same data
            if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1){
                die("sendto()");
            }
        //}while(difftime(end, start) <= DELAY );
        //send packets here


        //printf("elo\n");
    }




    //print connections
    print_connections(c, n);

    return 0;
}



/*
3
1.1.1.1/24 d 1
2.2.2.2/24 d 2
3.3.3.3/24 d 3
*/