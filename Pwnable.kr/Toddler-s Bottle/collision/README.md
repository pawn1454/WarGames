# Description
Daddy told me about cool MD5 hash collision today.
I wanna do something like that too!

ssh col@pwnable.kr -p2222 (pw:guest)

# Exploitation
```c
unsigned long check_password(const char* p){
	int* ip = (int*)p;
	int i;
	int res=0;
	for(i=0; i<5; i++){
		res += ip[i];
	}
	return res;
}
```
If you don't know what does that code do, I will tell you.

We give 20 bytes to this program, such as '11111111111111111111'.

Then this function adds each 4 bytes like:
```
31313131
31313131
31313131
31313131
31313131
res: f5f5f5f5
```

So I divided the hashcode number with 5, the result was 0x6C5CEC8.

'0x6C5CEC8'*5 is 5 less than the hashcode number, so the payload is:
```
---------------------------------
| '0x6C5CEC8' * 5 | '0x6C5CECC' |
---------------------------------
```

# Result
```bash
col@ubuntu:~$ ./col `python -c 'print "\xc8\xce\xc5\x06"*4+"\xcc\xce\xc5\x06"'`
daddy! I just managed to create a hash collision :)
col@ubuntu:~$ 
```
