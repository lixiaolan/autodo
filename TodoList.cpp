#include "TodoList.hpp"

void CopyBlankAutodoFile(string fileName) {
  unsigned int found = fileName.find_last_of("/");
  string blankFile = fileName.substr(0,found+1);
  blankFile = blankFile + "blankAutodo.xml";
  ifstream ifs(blankFile);
  ofstream ofs(fileName);
  string line;
  while (getline(ifs,line)) {
    ofs << line;
  }
  ifs.close();
  ofs.close();
};

int StringToInt(string in) {
  stringstream ss;
  ss << in;
  int i;
  ss >> i;
  return i;
};

string CharToString(const char_t * chs) {
  stringstream ss;
  ss << chs;
  return ss.str();
}

Todo::Todo(string tit,string sta, string due): title(tit), startdate(sta), duedate(due) {};

void Todo::AddNote(string note) {
  notes.push_back(note);
};

void Todo::BuildXml(xml_node n) {
  xml_attribute attr;
  n = n.append_child("todo");
  attr = n.append_attribute("title");
  attr.set_value(title.c_str());
  attr = n.append_attribute("duedate");
  attr.set_value(duedate.c_str());
  attr = n.append_attribute("startdate");
  attr.set_value(startdate.c_str());
  n.append_attribute("location") = "";
  n = n.append_child("infobox");
  n.append_attribute("type") = "notes";
  for (unsigned int i = 0; i < notes.size(); i++) {
    n.append_child("note");
  }
  n = n.child("note");
  for (unsigned int i = 0; i < notes.size(); i++) {
    attr = n.append_attribute("text");
    attr.set_value(notes[i].c_str());
    n = n.next_sibling("note");
  }
};

TodoList::TodoList(string a, string b) : fileName(a), userName(b) {
  //TODO: make this use pugixml also:
  ifstream ifs(fileName);
  if (!(ifs)) {
    cout << endl;
    cout << "Cannot locate autodo.xml file..." << endl;
    cout << "Creating blank autodo.xml file in install directory..." << endl;
    CopyBlankAutodoFile(fileName);
    cout << "Done!" << endl << endl;
  } 
  ifs.close();

  // Generate the xml string manually since the built in functinos DO NOT work.
  doc.load_file(fileName.c_str());
  IDMap = CleanList();

  // Load user in constructor
  user = GetUser(userName);
  SetUserCurrent(IDMap[GetUserCurrent()]);
};

map<int, int> TodoList::CleanList() {
  map<int, int> todoMap;
  stringstream ss;
  int i = 0;
  xml_attribute attr;

  for (xml_node n = doc.child("autodo").child("todo"); n; n = n.next_sibling("todo")) {
    ss.clear();
    ss.str(string());
    attr = n.attribute("id");
    if (attr) {
      //Does not check for overlaping IDs
      todoMap[StringToInt(n.attribute("id").value())] = i;
      ss << i;
      n.attribute("id").set_value(ss.str().c_str());
    }
    else {
      ss << i;
      attr = n.append_attribute("id");
      attr.set_value(ss.str().c_str());
    }
    if (!n.attribute("active")) {
      n.append_attribute("active") = "true";
    }
    i++;
  }
  return todoMap;
};

xml_node TodoList::GetUser(string u) {
  for (xml_node n = doc.child("autodo").child("user"); n; n = n.next_sibling("user")) {
    if (CharToString(n.attribute("name").value()) == u) {
      return n;
    }
  }
  cout << "ERROR: Could not find user" << endl;
  return user; 
};

void TodoList::PrintInfoBox(xml_node infobox) {
  // Subfunction for printing todobox.
  // This should handle types "notes", "files" and "links" for now.
  if (CharToString(infobox.attribute("type").value()) == "notes") {
    cout << "NOTES:\n";
    for (xml_node n = infobox.child("note"); n; n = n.next_sibling("note")) {
      cout << n.attribute("text").value() << endl;
    }
  }
};

void TodoList::PrintTodo(xml_node todo) {
  // This takes a pointer to a todo node in xml and prints it.
  
  // TODO: Include check to make sure pointer points to a todo node.
  // TODO: make the order of printing information fixed (title first, duedate second, etc...)
  
  xml_attribute attr = todo.attribute("title");
  cout << "TITLE: "<< attr.value() << "\n";
  attr = todo.attribute("duedate");
  cout << "DUE: "<< attr.value() << "\n";

  for (xml_node node = todo.child("infobox"); node; node = node.next_sibling("infobox")) {
    PrintInfoBox(node);
  }
};

void TodoList::PrintAutodoList() {
  for (xml_node n = doc.child("autodo").child("todo"); n; n = n.next_sibling("todo")) {
    cout << endl;
    PrintTodo(n);
    cout << endl;
  }
};

void TodoList::PrintAutodoListStartedAndActive() {
  for (xml_node n = doc.child("autodo").child("todo"); n; n = n.next_sibling("todo")) {
    if (CharToString(n.attribute("active").value()) == "true") {
      MyTime ST, T;
      ST.setDate(CharToString(n.attribute("startdate").value()));
      if (ST <= T) {
	cout << endl;
	PrintTodo(n);
	cout << endl;
      }
    }
  }
};

void TodoList::PrintAutodoListActiveAndDue(unsigned int plusNumDays) {
  for (xml_node n = doc.child("autodo").child("todo"); n; n = n.next_sibling("todo")) {
    if (CharToString(n.attribute("active").value()) == "true") {
      MyTime DT;
      MyTime T(plusNumDays);
      string temp = CharToString(n.attribute("duedate").value());
      DT.setDate(temp);
      if ((DT <= T) && (temp != "")) {
	cout << endl;
	PrintTodo(n);
	cout << endl;
      }
    }
  }
};

void TodoList::PrintTodoByID(string id) {
  for (xml_node n = doc.child("autodo").child("todo"); n; n = n.next_sibling("todo")) {
    if (CharToString(n.attribute("id").value()) == id) {
      PrintTodo(n);
    }
  }
};

void TodoList::LoadUser(string u) {
  user = GetUser(u);
  // Reset the users current todo if needed.
  SetUserCurrent(IDMap[GetUserCurrent()]);
};

void TodoList::PrintCurrent() {
  if (user != nullptr) {
    cout << endl;
    PrintTodoByID(user.attribute("currentTodo").value());
    cout << endl;
  }
};

void TodoList::NewTodo() {
  string yet = "true";
  vector<string> ids;
  for (xml_node n = doc.child("autodo").child("todo"); n; n = n.next_sibling("todo")) {
    if (CharToString(n.attribute("active").value())  == "true") {
      MyTime ST, T;
      ST.setDate(CharToString(n.attribute("startdate").value()));
      if (ST <= T) {
	ids.push_back(n.attribute("id").value());
      }
    } 
  }
  if (ids.size() > 1) {
    srand ( unsigned ( time(0) ) );
    random_shuffle ( ids.begin()+1, ids.end() );
    user.attribute("currentTodo").set_value(ids[1].c_str());
  } else {
    user.attribute("currentTodo").set_value("0");
  }
};

void TodoList::DeactivateCurrent() {
  if (user != nullptr) {
    string id = user.attribute("currentTodo").value();
    for (xml_node n = doc.child("autodo").child("todo"); n; n = n.next_sibling("todo")) {
      if (CharToString(n.attribute("id").value()) == id && CharToString(n.attribute("id").value()) != "0") {
	n.attribute("active").set_value("false");
      }
    }
  }
};

int TodoList::GetUserCurrent() {
  stringstream ss;
  int out;
  ss << user.attribute("currentTodo").value();
  ss >> out;
  return out;
};

void TodoList::SetUserCurrent(int in) {
  stringstream ss;
  ss << in;
  user.attribute("currentTodo").set_value(ss.str().c_str());
};

void TodoList::WriteFile() {
  doc.save_file(fileName.c_str());
};

void TodoList::AddTodo() {
  string tit, sta, due;
  
  cout << "Title: ";
  getline(cin, tit);
  cout << "Start: ";
  getline(cin, sta);
  cout << "Due: ";
  getline(cin, due);
  Todo T(tit, sta, due);
  string in1 = "dumb";
  while (in1 != "") {
    cout << "comment: ";
    getline(cin, in1);
    if (in1 == "") break;
    T.AddNote(in1);
  }

  T.BuildXml(doc.child("autodo"));

  IDMap = CleanList();
  user = GetUser(userName);
  SetUserCurrent(IDMap[GetUserCurrent()]);
};

void TodoList::AddComment() {

  vector<string> notes;
  string in = "dumb";

  while (in != "") {
    cout << "comment: ";
    getline(cin, in);
    if (in == "") break;
    notes.push_back(in);
  }

  xml_attribute attr;
  xml_node temp;
  string id = user.attribute("currentTodo").value();  
  for (xml_node n = doc.child("autodo").child("todo"); n; n = n.next_sibling("todo")) {
    if (CharToString(n.attribute("id").value()) == id) {
      for (xml_node m = n.child("infobox"); m; m.next_sibling("infobox")) {
	if (CharToString(m.attribute("type").value()) == "notes") {	  
	  for (unsigned int i = 0; i < notes.size(); i++) {
	    temp = m.append_child("note");
	    attr = temp.append_attribute("text");
	    attr.set_value(notes[i].c_str());
	  }
	  goto mylabel;
	};
      }
    }
  } 
 mylabel:
  return;
};
