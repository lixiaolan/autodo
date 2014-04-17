#ifndef XMLCURSOR
#define XMLCURSOR

#include <string>
#include <sstream>

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp" // for parsing xml file
#include "rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;

class XmlCursor {
private:
  xml_document<> *doc;
  xml_node<> *node;
  xml_attribute<> *attr;

  string XmlCursorCharToString(char * in);
public:

  xml_attribute<> * GetAttr();
  xml_node<> * GetNode();

  void SetDoc(xml_document<> *);

  void AddAttr(string att, string val);
  xml_node<> * AddNode(string type);
  void SetAttr(string n, string v);
  int GetAttrAsInt(string name);
  bool HasAttr(string at);

  XmlCursor & first_node();
  XmlCursor & first_node(string name);
  XmlCursor & next_sibling();
  XmlCursor & next_sibling(string name);
  XmlCursor & first_attribute(string name);
  XmlCursor & first_attribute();
  string attr_value();
  bool IsValad();

  XmlCursor & SetNode(xml_node<> * no);
  XmlCursor & SetNode();
};

#endif
