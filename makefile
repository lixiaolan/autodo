#Define the compile command to be used :
CC = x86_64-apple-darwin10.8.0-g++ -std=c++11
#Define the flags to be used with the compilestatment :
CFLAGS = -g -Wall
#Define your root directory:
ROOT = /Users/qile/Documents/programming/projects/Autodo/

#Define the various groups of .hpp and .o files used in compilation :

#(Not needed here)

#Define the rules in the dependancy tree :
autodo : $(ROOT)autodo.cpp TodoList.o AutodoVersionUpdate.o pugixml.o MyTime.o
	$(CC) -o $@ $^ $(CFLAGS)

TodoList.o : TodoList.cpp TodoList.hpp pugixml.o MyTime.o
	$(CC) -c -o $@ $< $(CFLAGS)

AutodoVersionUpdate.o : AutodoVersionUpdate.cpp AutodoVersionUpdate.hpp pugixml.o
	$(CC) -c -o $@ $< $(CFLAGS)

pugixml.o : pugixml.cpp pugixml.hpp pugiconfig.hpp
	$(CC) -c -o $@ $< $(CFLAGS)

MyTime.o : MyTime.cpp MyTime.hpp
	$(CC) -c -o $@ $< $(CFLAGS)

#Notes: $@ means "lhs of : ",
#	$^ means "rhs of : ",
#	$< means "first term on rhs of : ".x
