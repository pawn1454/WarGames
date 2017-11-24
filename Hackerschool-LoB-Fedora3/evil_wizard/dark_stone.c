/*
	The Lord of the BOF : The Fellowship of the BOF 
	- dark_stone
	- Remote BOF on Fedora Core 3 
	- hint : GOT overwriting again
	- port : TCP 8888
*/

#include <stdio.h>

// magic potion for you
void pop_pop_ret(void)
{
	asm("pop %eax");
	asm("pop %eax");
	asm("ret");
}
 
int main()
{
	char buffer[256];
	char saved_sfp[4];
	int length; 
	char temp[1024];

	printf("dark_stone : how fresh meat you are!\n");
	printf("you : ");
	fflush(stdout);

	// give me a food
	fgets(temp, 1024, stdin);

	// for disturbance RET sleding
	length = strlen(temp);
   
	// save sfp 
	memcpy(saved_sfp, buffer+264, 4);
 
	// overflow!!
	strcpy(buffer, temp);

	// restore sfp 
	memcpy(buffer+264, saved_sfp, 4);

        // disturbance RET sleding
        memset(buffer+length, 0, (int)0xff000000 - (int)(buffer+length));

	// buffer cleaning 
	memset(0xf6ffe000, 0, 0xf7000000-0xf6ffe000);

	printf("%s\n", buffer);
}
