# Description
Daddy, teach me how to use random value in programming!

ssh random@pwnable.kr -p2222 (pw:guest)

# Exploitation
```
pawn1454@ubuntu:~$ ./random 
1111
Wrong, maybe you should try 2^32 cases.
rand: 1804289383
key ^ random: 1804288304
pawn1454@ubuntu:~$ 
```
This program's rand value(1804289383) does not changed when I executed more than 3 times.

I should make (key ^ 1804289383) to be '0xdeadbeef'.

```
----------------------------------------------------
| 0110 1011 1000 1011 0100 0101 0110 0111 | rand   |
| 1011 0101 0010 0110 1111 1011 1000 1000 | key    |
| -------------------------------------------------|
| 1101 1110 1010 1101 1011 1110 1110 1111 | result |
----------------------------------------------------
```
The key binary in DEC is 3039230856.
```
random@ubuntu:~$ ./random 
3039230856
Good!
Mommy, I thought libc random is unpredictable...
random@ubuntu:~$ 
```
