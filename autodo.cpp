#include "TodoList.hpp"
#include "AutodoVersionUpdate.hpp"
#include "MyTime.hpp"

int main(int argc, char *argv[]) {
  stringstream ss;
  string s;

  string currFile = __FILE__;
  unsigned found = currFile.find_last_of("/");
  string currPath = currFile.substr(0,found+1);
  currPath = currPath + "autodo.xml";

  AutodoVersionUpdate(currPath);
  TodoList autodoList(currPath,"leland");
  
  for (int i = 1; i < argc; i++) {
    ss << argv[i] << " ";
  }
  
  ss >> s;
 
  if ( s == "-a" || s == "--add") {
    autodoList.AddTodo();
  }   
  
  if ( s == "-t" || s == "--todo" || argc == 1) {
    autodoList.PrintCurrent();    
  }

  if ( s == "-c" || s == "--comment" ) {
    autodoList.AddComment();    
    autodoList.PrintCurrent();    
  }

  if (s == "-l" || s == "--list") {
    unsigned u;
    if (ss >> u) {
      autodoList.PrintAutodoListActiveAndDue(u);
    } else {
      autodoList.PrintAutodoListStartedAndActive();
    }
  }

  if (s == "-n" || s == "--next") {
    autodoList.NewTodo();
    autodoList.PrintCurrent();    
  }

  if (s == "-d" || s == "--deactivate") {
    autodoList.DeactivateCurrent();
    autodoList.NewTodo();
    autodoList.PrintCurrent();
  }
  
  autodoList.WriteFile();
  return 0;
};
