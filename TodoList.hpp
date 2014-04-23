#ifndef TODO_LIST
#define TODO_LIST

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <fstream>

#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand

#include "pugixml.hpp"
#include "MyTime.hpp"

using namespace std;
using namespace pugi;

void CopyBlankAutodoFile(string fileName);

int StringToInt(string in);

string CharToString(const char_t* chs);

class Todo{
private:
  // This does not contain "active" or "id" since those are list qualities.
  string title;
  string startdate;
  string duedate;
  vector<string> notes;
public:
  Todo(string tit, string sta, string due);
  void AddNote(string note);
  void BuildXml(xml_node);
};

class TodoList {
private:
  // Strings to store file names.
  string fileName;
  string userName;
  
  xml_document doc;
  xml_node user;
  
  // This reads the Todo List and fixes all IDs
  void RelodeList(string);
  map<int, int> IDMap;
  //  /*Pinter to active todo*/ activeTodo;  
public:
  TodoList(string, string);  // Takes in file name and loads it
  map<int, int> CleanList();
  xml_node GetUser(string u);
  
  void ChooseNewTodo();
  void LoadUser(string);
  void NewTodo();
  void AddComment();
  void WriteFile();
  void DeactivateCurrent();
  
  void SetUserCurrent(int in);
  int GetUserCurrent();
  
  void PrintCurrent();
  void PrintTodoByID(string id);
  void PrintAutodoList();
  void PrintAutodoListStartedAndActive();
  void PrintAutodoListActiveAndDue(unsigned int plusNumDays);
  void PrintTodo(xml_node todo);
  void PrintInfoBox(xml_node infobox);
  
  void AddTodo();
};

#endif
