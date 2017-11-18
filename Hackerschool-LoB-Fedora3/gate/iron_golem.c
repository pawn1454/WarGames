/*
	The Lord of the BOF : The Fellowship of the BOF 
	- iron_golem
	- Local BOF on Fedora Core 3 
	- hint : fake ebp
*/
 
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
