all: rshell 

bin:
	[ ! -d bin ] && mkdir bin

rshell: bin  src/main.cpp
	g++ -Wall -Werror -ansi -pedantic src/main.cpp -o ./bin/rshell
