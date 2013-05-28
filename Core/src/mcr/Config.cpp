#include <mcr/Config.h>

#include <yaml-cpp/parser.h>
#include <yaml-cpp/node.h>
#include <mcr/io/FileStream.h>
#include <mcr/Debug.h>

namespace mcr {

bool Config::load(io::IFile* file, bool report)
{
    io::FileStream stream(file);
    std::istream fin(&stream);

    if (!fin)
    {
        if (report)
            debug("No config file found");

        return false;
    }

    try
    {
        YAML::Parser parser(fin);

        YAML::Node doc;
        parser.GetNextDocument(doc);
    
        for (auto it = doc.begin(); it != doc.end(); ++it)
        {
            std::string key, value;
            it.first()  >> key;
            it.second() >> value;

            if (report)
                debug("var %s = %s", key.c_str(), value.c_str());
            
            m_vars[key] = value;
        }
    }
    catch(YAML::ParserException& e)
    {
        if (report)
            debug("Yaml exception: %s", e.what());

        return false;
    }

    return true;
}

} // ns mcr
