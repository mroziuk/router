#include<stdio.h>	//printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>

#include "router.c"

int main(int argc, char const *argv[])
{
    struct connection c[3];
    char buff[50];
    int n = 1;
    // for (int i = 0; i < 3; i++)
    // {
    //     buff[i] = NULL;
    // }
    
    for (int i = 0; i < n; i++){
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
        //printf("wczytano %s %d\n", addr, d);
        // change str to bytes
        unsigned char bytes[5];
        fromStringToBytes(addr,bytes);
        c[i].address = bytes;
        c[i].distance = d;
        //print_as_bytes(c[i].address, 5);
        // write to connection

    }
    
    print_connections(c, n);
    return 0;
}
