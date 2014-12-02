RSHELL
======
###License BSD3

More information in LICENSE file

###Project available at: https://github.com/tassoeb/rshell

###Summary

This is a project for the course CS100/fall2014 at University of California, Riverside to perform some commands of a Linux Shell:
- bash 
- ls
- cp


Supervised by the Professor Mike Izbicki and Taeyoung Kim

###How to run

The project has a Makefile included.
Once you have the repository cloned just type 'make' and it will build a /bin folder and create executables files in it.

###Bugs
- RSHELL
- The input size is not unlimited. It has a maximum of 512 characters;
- Rshell will handle && and ||, in a order of precendence from left to right
- The "echo" command prints quotation marks: echo "hello world"; outputs "hello world"

- LS
- When the flag -R is set you have displayed all the path of a file, instead of a regular '.:'
