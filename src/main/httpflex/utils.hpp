#ifndef HTTPFLEX_UTILS_HPP
#define HTTPFLEX_UTILS_HPP

#include <iostream>
#include <vector>
#include <map>

namespace httpflex {
    void trim(std::string &s);

    std::vector<std::string> Split(const std::string &s, char delimiter);

    struct Header {
        std::string name;
        std::string value;
    };

    Header ParseHeader(std::string &header);

    std::map<std::string, std::string> ParseHeadersFromString(std::string &s);

}
#endif
