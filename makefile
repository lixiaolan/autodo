#Define the compile command to be used :
CC = x86_64-apple-darwin10.8.0-g++ -std=c++11
#Define the flags to be used with the compilestatment :
CFLAGS = -g -Wall
#Define your root directory:
ROOT = /Users/qile/Documents/programming/projects/Autodo/

#Define the various groups of .hpp and .o files used in compilation :

#(Not needed here)

#Define the rules in the dependancy tree :
autodo : $(ROOT)autodo.cpp TodoList.o XMLCursor.o
	$(CC) -o $@ $^ $(CFLAGS)

TodoList.o : TodoList.cpp TodoList.hpp XMLCursor.o
	$(CC) -c -o $@ $< $(CFLAGS)

XMLCursor.o : XMLCursor.cpp XMLCursor.hpp
	$(CC) -c -o $@ $< $(CFLAGS)

#Notes: $@ means "lhs of : ",
#	$^ means "rhs of : ",
#	$< means "first term on rhs of : ".x
