#include "TodoList.hpp"

void CopyBlankAutodoFile(string fileName) {
  unsigned found = fileName.find_last_of("/");
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

string CharToString(char * in) {
  stringstream ss;
  ss << in;
  return ss.str();
};

Todo::Todo(string tit, string due): title(tit), duedate(due) {};

void Todo::AddNote(string note) {
  notes.push_back(note);
};

void Todo::BuildXml(XmlCursor * cursor) {
  cursor->SetNode(cursor->AddNode("todo"));
  cursor->AddAttr("title",title);
  cursor->AddAttr("duedate",duedate);
  cursor->SetNode(cursor->AddNode("infobox"));
  cursor->AddAttr("type","notes");
  for (int i = 0; i < notes.size(); i++) {
    cursor->AddNode("note");
  }
  cursor->first_node("note");
  for (int i = 0; i < notes.size(); i++) {
    cursor->AddAttr("text",notes[i]);
    cursor->next_sibling("note");
  }
};

TodoList::TodoList(string a, string b) : fileName(a), userName(b) {
  ifstream ifs(fileName);

  if (!(ifs)) {
    cout << endl;
    cout << "Cannot locate autodo.xml file..." << endl;
    cout << "Creating blank autodo.xml file in install directory..." << endl;
    CopyBlankAutodoFile(fileName);
    cout << "Done!" << endl << endl;
    ifs.open(fileName);
  } 

  // Generate the xml string manually since the built in functinos DO NOT work.
  string xml;
  string line;
  
  while (getline(ifs,line))
    xml += line;

  vector<char> xml_copy(xml.begin(), xml.end());
  xml_copy.push_back('\0');
  autodoList.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);
  
  autodoListP = &autodoList;
  cursor.SetDoc(autodoListP);

  IDMap = CleanList();
  ifs.close();

  // Load user in constructor
  user = GetUser(userName);
  SetUserCurrent(IDMap[GetUserCurrent()]);
};

map<int, int> TodoList::CleanList() {
  map<int, int> todoMap;
  xml_attribute<> *attr;
  stringstream ss;
  int i = 0;
  
  for (cursor.SetNode().first_node("todo"); cursor.IsValad(); cursor.next_sibling("todo")) {
    ss.clear();
    ss.str(string());
    if (cursor.HasAttr("id")) {
      //Does not check for overlaping IDs
      todoMap[cursor.GetAttrAsInt("id")] = i;
      ss << i;
      cursor.SetAttr("id",ss.str());
    }
    else {
      ss << i;
      cursor.AddAttr("id",ss.str());
    }
    if (!cursor.HasAttr("active")) {
      cursor.AddAttr("active","true");
    }
    i++;
  }
  return todoMap;
};

xml_node<> * TodoList::GetUser(string u) {
  for (cursor.SetNode().first_node("user"); cursor.IsValad(); cursor.next_sibling("user")) {
    if (cursor.first_attribute("name").attr_value() == u) {
      return cursor.GetNode();  
    }
  }
  return nullptr;
};

void TodoList::PrintInfoBox(xml_node<> *infobox) {
  // Subfunction for printing todobox.
  // This should handle types "notes", "files" and "links" for now.
  if (CharToString(infobox->first_attribute()->value()) == "notes") {
    cout << "NOTES:\n";
    for (xml_node<> *node = infobox->first_node("note"); node; node = node->next_sibling("note")) {
      cout << node->first_attribute()->value() << endl;
    }
  }
};

void TodoList::PrintTodo(xml_node<> *todo) {
  // This takes a pointer to a todo node in xml and prints it.
  
  // TODO: Include check to make sure pointer points to a todo node.

  // Reads the attributes and prints relevant ones.
  // TODO: make the order of printing information fixed (title first, duedate second, etc...)
  for (xml_attribute<> *attr = todo->first_attribute(); attr; attr = attr->next_attribute()) {    
    if (CharToString(attr->name()) == "title") {
      cout << "TITLE: "<< attr->value() << "\n";
    }
    if (CharToString(attr->name()) == "duedate") {
      cout << "DUE: "<< attr->value() << "\n";
    }
  }
  for (xml_node<> *node = todo->first_node("infobox"); node; node = node->next_sibling("infobox")) {
    PrintInfoBox(node);
    // xml_node<> *node = todo->first_node("infobox");
  }
};

void TodoList::PrintAutodoList() {
  for (cursor.SetNode().first_node("todo"); cursor.IsValad(); cursor.next_sibling("todo")) {
    cout << endl;
    PrintTodo(cursor.GetNode());
    cout << endl;
  }
};

void TodoList::PrintTodoByID(string id) {
  for (cursor.SetNode().first_node("todo"); cursor.IsValad(); cursor.next_sibling("todo")) {
    if (cursor.first_attribute("id").attr_value() == id) {
      PrintTodo(cursor.GetNode());
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
    PrintTodoByID(CharToString(user->first_attribute("currentTodo")->value()));
    cout << endl;
  }
};

void TodoList::NewTodo() {
  string yet = "true";
  vector<string> ids;
  for (cursor.SetNode().first_node("todo"); cursor.IsValad(); cursor.next_sibling("todo")) {
    if (cursor.first_attribute("active").attr_value() == "true") {
      ids.push_back(cursor.first_attribute("id").attr_value());
    } 
  }
  if (ids.size() > 1) {
    srand ( unsigned ( time(0) ) );
    random_shuffle ( ids.begin()+1, ids.end() );
    cursor.SetNode(user).SetAttr("currentTodo",ids[1]);
  } else {
    cursor.SetNode(user).SetAttr("currentTodo","0");
  }
};

void TodoList::DeactivateCurrent() {
  if (user != nullptr) {
    string id = CharToString(user->first_attribute("currentTodo")->value());
    for (cursor.SetNode().first_node("todo"); cursor.IsValad(); cursor.next_sibling("todo")) {
      if (cursor.first_attribute("id").attr_value() == id && cursor.first_attribute("id").attr_value() != "0") {
	cursor.SetAttr("active","false");
      }
    }
  }
};

int TodoList::GetUserCurrent() {
  stringstream ss;
  int out;
  ss << user->first_attribute("currentTodo")->value();
  ss >> out;
  return out;
};

void TodoList::SetUserCurrent(int in) {
  stringstream ss;
  ss << in;
  cursor.SetNode(user).SetAttr("currentTodo", ss.str());
};

void TodoList::WriteFile() {
  ofstream ofs;
  ofs.open(fileName);
  ofs << autodoList;
  ofs.close();
};

void TodoList::AddTodo() {
  string in1, in2;
  
  cout << "Title: ";
  getline(cin, in1);
  cout << "Duedate: ";
  getline(cin, in2);
  Todo T(in1, in2);
  in1 = "dumb";
  while (in1 != "") {
    cout << "comment: ";
    getline(cin, in1);
    if (in1 == "") break;
    T.AddNote(in1);
  }

  cursor.SetNode();
  T.BuildXml(&cursor);

  IDMap = CleanList();
  user = GetUser(userName);
  SetUserCurrent(IDMap[GetUserCurrent()]);
};

void TodoList::AddComment() {
  string in;
  cout << "comment: ";
  getline(cin, in);
  
  string id = CharToString(user->first_attribute("currentTodo")->value());  
  
  for (cursor.SetNode().first_node("todo"); cursor.IsValad(); cursor.next_sibling("todo")) {
    if (cursor.first_attribute("id").attr_value() == id) break;
  }
  for (cursor.first_node("infobox"); cursor.IsValad(); cursor.next_sibling("infobox")) {
    if (cursor.first_attribute("type").attr_value() == "notes") break;
  }
  
  cursor.SetNode(cursor.AddNode("note"));
  cursor.AddAttr("text", in);
};
