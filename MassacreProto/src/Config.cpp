#include "Universe.h"
#include "Config.h"

#include <yaml-cpp/parser.h>
#include <yaml-cpp/node.h>

using namespace mcr;

std::map<std::string, std::string> Config::configuration;

void Config::load(const char* filename, FileSystem* fs)
{
    if (fs) {
        m_fs = fs;
        m_ownFs = false;
    }
    else {
        m_fs = new FileSystem;
        m_ownFs = true;
    }
    
    FileStream stream(m_fs->openFile(filename));
    try {
        std::istream fin(&stream);
        YAML::Parser parser(fin);

        YAML::Node doc;
        parser.GetNextDocument(doc);
        debug("Reading config file");
    
        for(YAML::Iterator it=doc.begin(); it!=doc.end(); ++it) {
            std::string key, value;
            it.first() >> key;
            it.second() >> value;
            debug("Type: %d", it.second().Type());
            debug("Key: %s, value: %s", key.c_str(), value.c_str());
            
            configuration.insert(std::make_pair(key, value));
    }
    }
    catch(YAML::ParserException& e) {
        debug("Yaml exception", e.what());
    }
}
