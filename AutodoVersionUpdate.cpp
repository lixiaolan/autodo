#include "AutodoVersionUpdate.hpp"

void AutodoVersionUpdate(string fileName) {
  // Current Max Version:
  unsigned int FileVersion;
  
  // Load up the document.
  // TODO: make sure the file exists.......

  xml_document doc;
  doc.load_file(fileName.c_str());
  xml_node n = doc.child("autodo");
  xml_attribute attr = n.attribute("version");
  
  if (attr) {    
    stringstream ss;
    ss << attr.value();
    ss >> FileVersion;
  } else {
    FileVersion = 0;    
  }

  switch (FileVersion) {
  case 0:
    n.append_attribute("version") = "1";
    cout << "autodo.xml updated to Version 1" << endl;
  case 1:
    n.attribute("version").set_value("2");
    for (xml_node m = n.child("todo"); m; m = m.next_sibling("todo")) {
      m.append_attribute("startdate") = "";
      m.append_attribute("location") = "";
    }
    cout << "autodo.xml updated to Version 2" << endl;
    
    // Make sure this is always in the LAST case statement. 
    doc.save_file(fileName.c_str());
  }
}
