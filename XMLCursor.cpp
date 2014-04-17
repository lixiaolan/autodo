#include "XMLCursor.hpp"

void XmlCursor::SetDoc(xml_document<> * d) {
  doc = d;
  SetNode();
};

xml_node<> * XmlCursor::GetNode() {
  return node;
};

xml_attribute<> * XmlCursor::GetAttr() {
  return attr;
};

string XmlCursor::XmlCursorCharToString(char * in) {
  stringstream ss;
  ss << in;
  return ss.str();
};

bool XmlCursor::HasAttr(string at) {
  attr = node->first_attribute(at.c_str());
  return !(attr==nullptr);
};

int XmlCursor::GetAttrAsInt(string name) {
  int i;
  stringstream ss;
  attr = node->first_attribute(name.c_str());
  ss << attr->value();
  ss >> i;
  return i;
};

void XmlCursor::SetAttr(string n, string v) {
  char *name = doc->allocate_string(v.c_str());
  node->first_attribute(n.c_str())->value(name);
};

xml_node<> * XmlCursor::AddNode(string type) {
  char *node_name = doc->allocate_string(type.c_str()); 
  xml_node<> * subNode = doc->allocate_node(node_element, node_name);
  node->append_node(subNode);
  return subNode;
};

void XmlCursor::AddAttr(string att, string val) {
  char * n = doc->allocate_string(val.c_str());
  char * a = doc->allocate_string(att.c_str());
  attr = doc->allocate_attribute(a, n);
  node->append_attribute(attr);
};

XmlCursor & XmlCursor::first_node() {
  node = node->first_node();
  return *this;
};

XmlCursor & XmlCursor::first_node(string name) {
  node = node->first_node(name.c_str());
  return *this;
};

XmlCursor & XmlCursor::next_sibling() {
  node = node->next_sibling();
  return *this;
};

XmlCursor & XmlCursor::next_sibling(string name) {
  node = node->next_sibling(name.c_str());
  return *this;
};

XmlCursor & XmlCursor::first_attribute() {
  attr = node->first_attribute();
  return *this;
};

XmlCursor & XmlCursor::first_attribute(string name) {
  attr = node->first_attribute(name.c_str());
  return *this;
};

string XmlCursor::attr_value() {
  return XmlCursorCharToString(attr->value());
};

bool XmlCursor::IsValad() {
  return !(node == nullptr);
}

XmlCursor & XmlCursor::SetNode() {
  node = doc->first_node();
  return *this;
};

XmlCursor & XmlCursor::SetNode(xml_node<> * no) {
  node = no;
  return *this;
};
