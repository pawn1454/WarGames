# Description
Papa brought me a packed present! let's open it.

Download : http://pwnable.kr/bin/flag

This is reversing task. all you need is binary

# Exploitation
```
pawn1454@ubuntu:~$ upx -d ./flag 
                       Ultimate Packer for eXecutables
                          Copyright (C) 1996 - 2013
UPX 3.91        Markus Oberhumer, Laszlo Molnar & John Reiser   Sep 30th 2013

        File size         Ratio      Format      Name
   --------------------   ------   -----------   -----------
    887219 <-    335288   37.79%  linux/ElfAMD   flag

Unpacked 1 file.
pawn1454@ubuntu:~$
```
It is packed with UPX, so I unpacked it.

And use gdb to know what is the flag.

```
RCX: 0x8 
RDX: 0x496628 ("UPX...? sounds like a delivery service :)")
RSI: 0x0 
```
