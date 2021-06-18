#include "pugixml.hpp"
#include <iostream>
#include "XMLReaderTest.h"
#include"engine_constants.h"
void printSomeShitFromXML()
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file((ASSETS_ROOT_DIR+std::string("xgconsole.xml")).c_str());
    if (!result)
    {
        std::cout <<"Cant open File";
        return;
    }
    for (pugi::xml_node tool : doc.child("Profile").child("Tools").children("Tool"))
    {
        int timeout = tool.attribute("Timeout").as_int();

        if (timeout > 0)
            std::cout << "Tool " << tool.attribute("Filename").value() << " has timeout " << timeout << "\n";
    }
}