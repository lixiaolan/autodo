To install autodo, follow these instructions:

1.) Open makefile
2.) Adjust CC to match your c++11 compliant c++ compiler call.
3.) Change ROOT to be the path to the your install directory.
4.) Save your changes.
5.) run make.
6.) Run ./autodo

USAGE:

autodo    : show current todo
autodo -n : next todo (skip the current one)
autodo -a : add new todo
autodo -c : add comment to current
autodo -d : deactivate current todo (check it off the list) 
autodo -l : list all todos on file 


