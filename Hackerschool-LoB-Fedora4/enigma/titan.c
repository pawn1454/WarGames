/*
        The Lord of the BOF : The Fellowship of the BOF
        - titan
        - Remote BOF on Fedora Core 4
        - hint : ? 
	- port : TCP 8888
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static char buffer[40];
static void (*ftn)();

void print()
{
        printf("nothing here\n");
	fflush(stdout);
}

int main()
{
        char buf[48];
        ftn = print;

        printf("titan : What a tragic mistake.\n");
        printf("you : ");
        fflush(stdout);

	// give me a food
        fgets(buf,48,stdin);

	// buffer overflow!!
        strcpy(buffer,buf);

	// preventing RTL
        if(((int)ftn & 0xff000000) == 0)
        {
                printf("I've an allergy to NULL");
                exit(1);
        }

	// clearing buffer
	memset(buffer, 0, 40);

        ftn();
}
