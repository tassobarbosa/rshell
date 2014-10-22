RSHELL
======
###License BSD3

More information in LICENSE file

###Project available at: https://github.com/tassoeb/rshell

###Summary

This is a project for the course CS100/fall2014 at University of California, Riverside to perform some commands of a Linux Shell called by the function execvp.

Supervised by the Professor Mike Izbicki and Taeyoung Kim

###How to run

The project has a Makefile included.
Once you have the repository cloned just type 'make' and it will build a /bin folder and create a executable file in it.

###Bugs

- The input size is not unlimited. It has a maximum of 512 characters;
- Rshell will handle && and ||, in a order of precendence from left to right, if commands are separated (blank space) as shown in the example: ls -a && pwd
- The "echo" command prints quotation marks: echo "hello world"; outputs "hello world"
