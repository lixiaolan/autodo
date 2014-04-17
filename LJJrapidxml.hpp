#ifndef LJJ_RAPID_XML
#define LJJ_RAPID_XML

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

void rapidxml_append_attribute(xml_document<> *doc, xml_node<> *node, string tag, string value) {
    char *name;
    xml_attribute<> *attr;
 
    name = doc->allocate_string(value.c_str());
    attr = doc->allocate_attribute(tag, name);
    node->append_attribute(attr);
}


#endif
