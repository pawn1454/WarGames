/*
	The Lord of the BOF : The Fellowship of the BOF 
	- hell_fire
	- Remote BOF on Fedora Core 3 
	- hint : another fake ebp or got overwriting 
	- port : TCP 7777
*/

#include <stdio.h>

int main()
{
	char buffer[256];
	char saved_sfp[4];
	char temp[1024];
  
	printf("hell_fire : What's this smell?\n");
	printf("you : ");
	fflush(stdout);

	// give me a food
	fgets(temp, 1024, stdin);
   
	// save sfp 
	memcpy(saved_sfp, buffer+264, 4);
 
	// overflow!!
	strcpy(buffer, temp);

	// restore sfp 
	memcpy(buffer+264, saved_sfp, 4);

	printf("%s\n", buffer);
}
