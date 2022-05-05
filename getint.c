#include<stdio.h>	//printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
int main(int argc, char const *argv[]){
    // // read connections from stdandard input
    // char buff[50];
    // int n;
    // //scanf("%d", &n);
    // char nbuff[10];
    // fgets(nbuff, 10, stdin);
    // n = atoi(nbuff);
    
    // printf("%s %d", nbuff, n);
    char *dest;
    char src[6] = "abcde";

    strncpy(dest, src, 5);
    printf("%.5s", dest);
}