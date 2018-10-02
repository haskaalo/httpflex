#include "httpflex/utils.hpp"

#include <iostream>
#include <vector>
#include <set>
#include <sstream>

namespace httpflex {
    std::vector<std::string> Split(const std::string &s, char delimiter)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);

        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }

        return tokens;
    }

    Header ParseHeader(std::string &header)
    {
        Header headers;
        unsigned long pos = header.find(':');
        if (pos == -1) {
            throw std::runtime_error("Invalid header format");
        }
        if (pos + 1 > header.size()) {
            throw std::runtime_error("Invalid header format");
        }

        headers.name = header.substr(0, pos);
        headers.value = header.substr(pos+1, header.size());

        trim(headers.name);
        trim(headers.value);

        return headers;
    }

    std::map<std::string, std::string> ParseHeadersFromString(std::string &s)
    {
        std::istringstream ss(s);
        std::string line;
        bool firstLine = true;
        Header header;
        std::map<std::string, std::string> headers;

        while (std::getline(ss, line, '\r')) {
            if (firstLine) { // Ignore first line which should be an empty line
                firstLine = false;
                continue;
            }

            if (line.empty()) {
                continue;
            }

            if (line.size() == 1) {
                continue;
            }

            try {
                line.erase(0, 1); // TODO: First char is linebreak. Why?
                header = ParseHeader(line);
            } catch (std::runtime_error& err) {
                throw;
            }
            
            headers.insert({header.name, header.value});
        }

        return headers;
    }
}
