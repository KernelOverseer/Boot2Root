## Boot2Root Writeup 2

### We will follow Writeup 1, and stop when we get laurie's credentials.
We will use the `DirtyCow` exploit.
This exploit will permit us to write on a file we don't have permission on.
For example `/etc/passwd`
We can change the line
`root:x:0:0:root:/root:/bin/bash`
And replace the `x` by our crypted password

`dirtycow.c` script provided in the `scripts` directory

And voila we have changed the password for root and can access it using `su root`
