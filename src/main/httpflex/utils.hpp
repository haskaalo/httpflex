#ifndef HTTPFLEX_UTILS_HPP
#define HTTPFLEX_UTILS_HPP

#include <iostream>
#include <vector>
#include <set>

namespace httpflex {
    std::vector<std::string> Split(const std::string &s, char delimiter);


    struct Header {
        std::string name;
        std::string value;
    };

    Header ParseHeader(const std::string &header);
}
#endif
