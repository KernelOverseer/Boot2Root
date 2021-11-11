## Boot2Root Writeup 3

We can interrupt the boot, by pressing `Ctrl-Alt-F1`
We will have a prompt like this
```
boot:
```
What happens after the kernel is loaded is that it needs to give control to a user-space program usually called `init`, `init` then  runs some scripts to set up various things (network devices, mount partitions, etc...).
Then a programm like `getty` will watch for a `tty` connection an ask for login.

by specifying:
```
boot: live /boot/ init=/bin/sh
```
We make the kernel directly run `/bin/sh` as `root`
so we don't get asked for login.
And now we are root !
