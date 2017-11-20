/*
	The Lord of the BOF : The Fellowship of the BOF 
	- dark_eyes
	- Local BOF on Fedora Core 3 
	- hint : RET sleding
*/

int main(int argc, char *argv[])
{
	char buffer[256];
	char saved_sfp[4];
    
	if(argc < 2){
		printf("argv error\n");
		exit(0);
	}
   
	// save sfp 
	memcpy(saved_sfp, buffer+264, 4);
 
	// overflow!!
	strcpy(buffer, argv[1]);

	// restore sfp 
	memcpy(buffer+264, saved_sfp, 4);

	printf("%s\n", buffer);
}
