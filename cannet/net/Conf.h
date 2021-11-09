#ifndef CANNET_CONF_H
#define CANNET_CONF_H

#include <list>
#include <map>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <memory>

namespace cannet {
    class Conf {
        int parse(const std::string filename);

        std::string get(std::string section, std::string name, std::string default_value);

        long getInteger(std::string section, std::string name, long default_value);

        double getFloat(std::string section, std::string name, double default_value);

        bool getBoolean(std::string section, std::string name, bool default_value);

        std::list<std::string> getStrings(std::string section, std::string name);

        std::map<std::string, std::list<std::string>> values_;

        std::string filename;
    };
}


#endif //CANNET_CONF_H
