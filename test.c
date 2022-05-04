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
