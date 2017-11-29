# Description
Mommy! what is a file descriptor in Linux?

* try to play the wargame your self but if you are ABSOLUTE beginner, follow this tutorial link: https://www.youtube.com/watch?v=blAxTfcW9VU

ssh fd@pwnable.kr -p2222 (pw:guest)

# Exploitation
```
int fd = atoi( argv[1] ) - 0x1234;
int len = 0;
len = read(fd, buf, 32);
if(!strcmp("LETMEWIN\n", buf)){
  printf("good job :)\n");
  system("/bin/cat flag");
  exit(0);
}
```
In `fd.c`, Asks you know fd or not, It sets fd with (argv[1] - 0x1234).

And It calls read function with the fd option.

To read buf 32bytes successfully, I have to set fd to 0.

It means (atoi(argv[1]) - 0x1234) must be 0.

0x1234 is 4660(10).

# Payload
```
fd@ubuntu:~$ ./fd 4660
LETMEWIN
good job :)
mommy! I think I know what a file descriptor is!!
fd@ubuntu:~$ 
```
