#include "httpflex/utils.hpp"

#include <iostream>
#include <vector>
#include <set>
#include <sstream>


std::vector<std::string> Split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);

   while (std::getline(tokenStream, token, delimiter)) {
      tokens.push_back(token);
   }

   return tokens;
}

Header ParseHeader(const std::string& header)
{
    Header headers;

    int pos = header.find_first_of(':');
    if (pos == -1) {
        throw std::runtime_error("Invalid header format");
    }
    if (pos+1 > header.size()) {
        throw std::runtime_error("Invalid header format");
    }

    headers.name = header.substr(pos+1);
    headers.value = header.substr(0, pos);

    return headers;
}
