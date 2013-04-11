#include "Universe.h"
#include "Config.h"

#include <yaml-cpp/parser.h>
#include <yaml-cpp/node.h>

namespace mcr {

std::map<std::string, std::string> Config::s_configuration;

void Config::load(IFile* file)
{
    FileStream stream(file);
    try
    {
        std::istream fin(&stream);
        YAML::Parser parser(fin);

        YAML::Node doc;
        parser.GetNextDocument(doc);
        debug("Reading config file");
    
        for(YAML::Iterator it=doc.begin(); it!=doc.end(); ++it)
        {
            std::string key, value;
            it.first()  >> key;
            it.second() >> value;

            debug("Type: %d", it.second().Type());
            debug("Key: %s, value: %s", key.c_str(), value.c_str());
            
            s_configuration[key] = value;
        }
    }
    catch(YAML::ParserException& e)
    {
        debug("Yaml exception: %s", e.what());
    }
}

} // ns mcr
