#include<stdio.h>	//printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
void print_as_bytes (unsigned char* buff, ssize_t length)
{
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
int arrayToint(char arr[]){
    return (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];
}
void intToArray(int x, char arr[]){
    arr[0] = (x >> 24) & 0xFF;
    arr[1] = (x >> 16) & 0xFF;
    arr[2] = (x >>  8) & 0xFF;
    arr[3] = (x      ) & 0xFF;
}

int main(int argc, char const *argv[])
{

    char arr[4];
    int n = 4097 + 1;
    intToArray(n, arr);

    print_as_bytes(arr, 4);
    printf("%d", arrayToint(arr));
    return 0;
}


// int main(int argc, char const *argv[]){
//     unsigned char buff[] = {192,168,0,1,24};
//     // print_as_bytes(buff, 4);
//     // char res[16];
//     // fromBytesToSrting(buff, 5, res);
//     // printf("%s",res);
//     char str[] = "192.168.0.1/24";
//     fromStringToBytes(str, buff);
//     for(int i=0;i<5;i++){
//         printf("%d ",buff[i]);
//     }
//     print_as_bytes(buff, 5);
//     return 0;
// }
