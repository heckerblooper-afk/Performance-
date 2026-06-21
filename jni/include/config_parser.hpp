#pragma once

#include "perf_suite.hpp"
#include <string>
#include <map>

class ConfigParser {
public:
    bool parseFile(const std::string &filepath, AppConfig &config);
    
private:
    void applyConfigValue(const std::string &key,
                         const std::string &value,
                         AppConfig &config);
    
    void parseGameProps(const std::string &props_str,
                        std::map<std::string, std::string> &props);
};
