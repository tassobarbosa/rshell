all: rshell ls

bin:
	[ ! -d bin ] && mkdir bin

rshell: bin  src/main.cpp
	g++ -Wall -Werror -ansi -pedantic src/main.cpp -o ./bin/rshell

ls: bin src/ls.cpp
	g++ src/ls.cpp -o ./bin/ls



