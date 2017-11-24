/*
	The Lord of the BOF : The Fellowship of the BOF 
	- evil_wizard
	- Local BOF on Fedora Core 3 
	- hint : GOT overwriting
*/

// magic potion for you
void pop_pop_ret(void)
{
	asm("pop %eax");
	asm("pop %eax");
	asm("ret");
}
 
int main(int argc, char *argv[])
{
	char buffer[256];
	char saved_sfp[4];
	int length; 

	if(argc < 2){
		printf("argv error\n");
		exit(0);
	}

	// for disturbance RET sleding
	length = strlen(argv[1]);
   
        // healing potion for you
        setreuid(geteuid(), geteuid());
        setregid(getegid(), getegid());

	// save sfp 
	memcpy(saved_sfp, buffer+264, 4);
 
	// overflow!!
	strcpy(buffer, argv[1]);

	// restore sfp 
	memcpy(buffer+264, saved_sfp, 4);

        // disturbance RET sleding
        memset(buffer+length, 0, (int)0xff000000 - (int)(buffer+length));

	printf("%s\n", buffer);
}
