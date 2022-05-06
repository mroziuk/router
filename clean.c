#include<stdio.h>	//printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include <time.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <sys/types.h>
#include <stdbool.h>
#define BUFLEN 9	//Max length of buffer
#define PORT 54321	//The port on which to send data
#define DELAY 2.0
#define MAX_NOT_ANSWERED 10
void print_as_bytes (unsigned char* buff, ssize_t length){
	for (ssize_t i = 0; i < length; i++, buff++)
		printf ("%.2x ", *buff);	
}

void fromBytesToSrting(unsigned char* buff, ssize_t length, char res[]){
    sprintf(res, "%d.%d.%d.%d/%d", buff[0],buff[1],buff[2],buff[3],buff[4]);
}
void fromBytesToSrtingWithoutMask(unsigned char* buff, ssize_t length, char res[]){
    sprintf(res, "%d.%d.%d.%d", buff[0],buff[1],buff[2],buff[3]);
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
    int connected_directly;
    unsigned char via[5];
    int time_from_last_recived;
};
void die(char *s){
	perror(s);
	exit(1);
}

void print_connections(struct connection cons[], int n){
    for(int i=0;i<n;i++){
        char addr[30], via[30];
        fromBytesToSrting(cons[i].address, 5, addr);
        fromBytesToSrtingWithoutMask(cons[i].via, 5, via);
        if(cons[i].connected_directly == 0){
            printf("%s distance %d via %s, l:%d\n", addr, cons[i].distance, via, cons[i].time_from_last_recived);
        }else{
            printf("%s distance %d connected directly\n", addr, cons[i].distance);
        }
        
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
        c[i].connected_directly = 1;
        c[i].time_from_last_recived = 0;
    }
}
int arrayToint(char arr[]){
    return (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];
}
void intToArray(int x, char arr[]){
    arr[0] = (x >> 24) & 0xFF;
    arr[1] = (x >> 16) & 0xFF;
    arr[2] = (x >>  8) & 0xFF;
    arr[3] = (x      ) & 0xFF;
}

void increase_time_from_last_message(struct connection c[], int n){
    for(int i=0;i<n;i++){
        c[i].time_from_last_recived += 1;
    }
}
void update_time_from_last_message(struct connection c[], int n, char addr[]){
    for(int i=0;i<n;i++){
        if(memcmp(c[i].address, addr, 4)== 0){
            c[i].time_from_last_recived = 0;
        }
    }
}
void swap(void* a, void* b, int n){
    unsigned char* p;
    unsigned char* q;
    unsigned char* const sentry = (unsigned char*)a + n;

    for(p = a, q = b; p < sentry; ++p, ++q){
        const unsigned char t = *p;
        *p = *q;
        *q = t;
    }
}
void remove_unreachable_network(struct connection c[], int *n){
    for(int i=0;i<*n;i++){
        if(c[i].time_from_last_recived > MAX_NOT_ANSWERED){
            swap(&c[i],&c[*n-1], sizeof(struct connection));
            *n -= 1;
            return; 
        }
    }
}
int main(int argc, char const *argv[]){
    //create array of connections
    int n, m = 0;
    struct connection c[100];
    struct connection cr[100];
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
    //
    int flags = fcntl(s, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(s, F_SETFL, flags);
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
    // client side configuration
    memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);

	char server_address[20];
    fromBytesToSrtingWithoutMask(c[0].address,5,server_address);
	if (inet_aton(server_address , &si_other.sin_addr) == 0) 
	{
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}
    // end of cliewnt

    time_t start, end;        
    while(1){
        increase_time_from_last_message(c,n);
        print_connections(c,n);
        print_connections(cr,m);
        time(&start);
        do{
            time(&end);
            //get all packets
            while( recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) > 0){
                // printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
                // print_as_bytes(buf, 5);
                // printf("%d\n", arrayToint(buf+5));
                //process recieved data
                //jesli jest taki addres sprawdz odleglosc
                char sender_addr[5]; // 
                fromStringToBytes(inet_ntoa(si_other.sin_addr),sender_addr);
                update_time_from_last_message(c,n,sender_addr);

                int address_exists = 0; // false
                for(int i=0;i<n;i++){
                    if(memcmp(buf,c[i].address, 4) == 0){// strings are equal
                        address_exists = 1; //true
                    } 
                }
                for(int i=0;i<m;i++){
                    if(memcmp(buf,cr[i].address, 4) == 0){// strings are equal
                        address_exists = 1; //true
                    } 
                }
                if(address_exists == 0){ //address doesnt eists
                    //add new address
                    memcpy(cr[m].address, buf, 5);
                    cr[m].connected_directly = 0;
                    // find distance of matching addr
                    for(int i=0;i<n;i++){
                        if(memcmp(sender_addr,c[i].address,4) == 0){
                            cr[m].distance = arrayToint(buf+5) +  c[i].distance;
                            memcpy(cr[m].via, sender_addr, 5);
                        }
                    }
                    m += 1;
                }
            }
            
            //printf("waited enough");
        }while(difftime(end, start) <= DELAY );
        //send packets here
            int broadcast=1;
            if (setsockopt(s,SOL_SOCKET,SO_BROADCAST,
                &broadcast,sizeof(broadcast))==-1) {
                die("setsockopt");
            }

            for(int i=0;i<n;i++){
                unsigned char message[10];
                memcpy(message, c[i].address, 5);
                //fromBytesToSrtingWithoutMask(c[i].address, 5, message);
                intToArray(c[i].distance, message+5);
                if (sendto(s,message, 9, 0 , (struct sockaddr *) &si_other, slen)==-1)
                {
                    die("sendto()");
                }
            }
            
        //
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

/*
git add . && git commit -m "change" && git push
*/
