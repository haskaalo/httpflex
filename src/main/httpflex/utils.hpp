#ifndef HTTPFLEX_UTILS_HPP
#define HTTPFLEX_UTILS_HPP

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

namespace httpflex {
    std::vector<std::string> Split(const std::string &s, char delimiter);


    struct Header {
        std::string name;
        std::string value;
    };

    Header ParseHeader(std::string &header);

    std::map<std::string, std::string> ParseHeadersFromString(std::string &s);

    // trim from start (in place)
    inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }

}
#endif
