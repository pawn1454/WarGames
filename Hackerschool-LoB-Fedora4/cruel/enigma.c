/*
        The Lord of the BOF : The Fellowship of the BOF
        - enigma
        - Remote BOF on Fedora Core 4
        - hint : ? 
	- port : TCP 7777
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int vuln(int canary,char *ptr)
{
        char buffer[256];
        int *ret;
        
	// stack overflow!!
	strcpy(buffer,ptr); 

	// overflow protected
        if(canary != 0x31337)
        {
                printf("who broke my canary?!");
                exit(1);
        }

        // preventing RTL
        ret = &canary - 1;
        if((*ret & 0xff000000) == 0) 
        {
                printf("I've an allergy to NULL");
                exit(1);
        }

	// clearing attack buffer
	memset(ptr, 0, 1024);

        return 0;
}

int main()
{
	char buffer[1024];

	printf("enigma : The brothers will be glad to have you!\n");
	printf("you : ");
	fflush(stdout);

	// give me a food!
        fgets(buffer, 1024, stdin);

	// oops~!
        vuln(0x31337, buffer);
	
	// bye bye
	exit(0);
}
