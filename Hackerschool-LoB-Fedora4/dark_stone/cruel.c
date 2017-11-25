/*
	The Lord of the BOF : The Fellowship of the BOF 
	- cruel
	- Local BOF on Fedora Core 4
	- hint : no more fake ebp, RET sleding on random library
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main(int argc, char *argv[])
{
    char buffer[256];

    if(argc < 2){
        printf("argv error\n");
        exit(0);
    }

    strcpy(buffer, argv[1]);
    printf("%s\n", buffer);
}
