# linux-systems-programming
Various list of linux programming exercises for someone to ramp up on System Programming basics in C on a linux box.

Here are the tools to install (gcc, make and cmake):

```
$ sudo apt-get install build-essential
$ sudo apt-get install cmake
```

Install, Build and Execute the Examples
============
Follow the below steps to install and build the repo:

```
$ git clone https://github.com/ashishbajaj99/linux-systems-programming
$ cd linux-systems-programming
$ mkdir build
$ cd build
$ CC=gcc CXX=g++ cmake ..
$ make # build all the examples
$ make install # move all test vectors into the respective executable directories
```
Now each individual program is available in the build directory in the respective folder. Below is example execution
```
$ cd 01 && ./prog1 && cd ..
$ cd 02 && ./prog2 foo && cd ..
$ cd 03 && ./prog3 foo && cd ..
$ cd 04 && time ./prog4a 1 1000000 && cd ..
$ cd 04 && time ./prog4b 1 1000000 && cd ..
$ cd 05 && ./gendatafile && ./prog5 && cd ..
$ cd 06 && ./prog6 && cd ..
$ ./05/gendatafile && mv datafile 07/ && cd 07 && ./prog7 && cd ..
$ cd 08 && ./prog8 prog8 && cd ..
$ cd 09 && ./prog9 test && cd ..
$ cd 10 && ./prog10 ~ && cd ..
$ cd 11 && ./prog11 ~ && cd ..
$ cd 12 && ./prog12 && cd .. # you can modify/delete foo and bar and see that the program is monitoring these files
$ cd 13 && ./prog13 hello, this is a crude echo program  && cd ..
$ cd 14 && ./prog14 && cd ..
$ cd 15 && ./prog15 -cvb 10 -f filename && cd ..
$ cd 16 && ./prog16 && cd ..
$ cd 17 && ./prog17 -ar ../../ && cd ..
$ cd 18 && ./prog18 && cd .. # warning this program writes a large file to filesystem to measure its time, please wait
$ cd 19 && ./prog19 && cd .. # its normal to require to hit a carriage return to get back the prompt
$ cd 20 && ./prog20 && cd .. # its normal to require to hit a carriage return to get back the prompt
$ cd 21 && ./prog21 && cd ..
$ cd 22 && ./prog22 && cd ..
$ cd 23 && ./prog23
$ cd ..
$ cd 24 && ./prog24 && cd ..
$ cd 25 && ./prog25
$ cd ..
$ cd 26 && ./prog26 && cd ..
$ cd 27 && ./prog27 # keep another terminal window open to send kill -9 signal to pid
$ cd ..
$ cd 28 && ./prog28 # sending Ctrl-C SIGINT signal will show the error from pipe, kill process with Ctrl-\ SIGQUIT
$ cd ..
$ cd 29 && ./prog29 # sending Ctrl-C SIGINT signal will *not* show the error from pipe, kill process with Ctrl-\ SIGQUIT
$ cd ..
$ cd 30 && ./prog30 # send SIGHUP after manually changing reconfigure.conf to see the changes reflecting
$ cd ..
$ cd 31 && ./prog31 # if you wait 5 seconds, you will get timeout message
$ cd ..
$ ./32/prog32
$ ./33/prog33
```

Exercise List
==========
1. Write a program that writes hello world into a file using low level I/O. Ensure proper error handling is performed on the open() and write() function calls.
2. Write a file copy program that copies contents from foo to bar using low level I/O – ensure proper error checking is in place.
3. Write a file copy program that copies contents from foo to bar using standard C file I/O – ensure proper error checking is in place.
4. Write two programs prog4a (using standard C file I/O) and prog4b (using low level file I/O) that take in two arguments, blocksize and blockcount and write out blockcount number of times a buffer of size blocksize to a file. Compute the time taken by each of the programs with varied values of blocksize and blockcount (e.g. blocksize=1, blockcount=10000000, blocksize=1000, blockcount=1000000, etc.)
5. Write a program to open a file and modify the 2nd record contained within a datafile. The file format should contain records of type int id; char name[80]. Use only low-level I/O (open, lseek, read, write) with error checking to demonstrate this capability.
6. Write a program to perform file copy using Memory Mapped I/O using mmap() and memcpy() and msync(). Handle all error conditions properly.
7. Write a program to modify the 2nd record contains within a datafile using memory mapped I/O
8. Write a program to print out the last modified, last accessed and last changed attributes of an input file
9. Write a program to print output file attributes similar to ls –l on a single file
10. Write a program to print output file attributes similar to ls –l on a directory 
11. Write a program to add up the total size of files in a given directory
12. Write a program to monitor first 100 files mentioned in a monitor.conf file for modification and deletion and report all changes to a monitor.log file
13. Write a program that prints out all the arguments input to it (similar to echo)
14. Write a program that prints out the entire environment for the process
15. Write a program that takes ‘c’, ‘v’, as Boolean options, ‘b’ as a numeric option, ‘f’ as a text option and remaining non-option arguments and prints out all the options that were set along with non-option arguments. Test the program to work with various ordering of all the options, combining options, etc.
16. Write a program to use ctime() and strfmtime()
17. Write a program to recursively traverse a directory and build a hour by hour historogram that tracks the last modification time of regular files and prints out the histogram.
18. Write a program to measure the time taken by a process using sysconf(), times() and clock()
19. Write a program to create a child process, the parent process prints PARENT + pid, and the child process prints CHILD + pid and both processes exit.
20. Write a program to create a child process, the parent process prints PARENT + pid 10000 times, and the child process prints CHILD + pid 10000 times and both processes exit.
21. Write a program where a parent forks a child process and then blocks itself till the child terminates, after which it prints out if the child exited normally OR if the child was terminated. The child thread should simply print its own process ID and sleep for 500 seconds to allow for signals to be sent to it from the terminal to test the various status messages.
22. Write a program that gives a choice to the user to either run ps, ls, date and goof and run the chosen command using execlp()
23. Write a small shell program that has a command prompt “ashish> ” and uses execlp() to execute any inputs provided by the user
24. Write a program where the execlp() is used along with parent and child with connected pipes to achieve `ls | sort –r`
25. Write a program that extends the shell program written in #23 to support a shell that can handle inputs any commands input with a max of one pipe.
26. Write a program that lists first 32 signals available in the system
27. Write a program that has a global counter that increments every 1 second and the counter resets every time there is a SIGINT signal. SIGQUIT and SIGTERM should be ignored. Remember to manage critical regions in the main loop.
28. Write a program where you register a signal handler using the signalaction() with sa_flags=0 and perform a read operation on a pipe. Show that receiving a signal, stops the pipe read mid-way and is unable to resume it.
29. Write a program to show that using sigaction() the program written in #28 can be modified 
30. Write a program to re-read a reconfigure.conf file everytime a SIGHUP signal is received. Using the values in the reconfigure.conf file the program should be able to change its behavior without actually requiring a termination and restart.
31. System calls such as read() are blocking and do not timeout (they can block forever waiting for data) – use the SIGALRM signal that gets raised by the alarm() system call to print a message to the screen when the user doesn’t enter any input for longer than 5 seconds.
32. Write a TCP echo server listening on port 8000, i.e. it replies back with the same text that the client sends to it. Test it out with three concurrent telnet connections and check its working with lsof and wireshark.
33. Extend the program #32 to behave like a rot13 server (rotate each character by 13, instead of a simple echo server.


License
==========
The MIT License (MIT)

Copyright (c) 2016 Ashish Bajaj

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
