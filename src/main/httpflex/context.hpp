#include <map>

#ifndef HTTPFLEX_CONTEXT_HPP
#define HTTPFLEX_CONTEXT_HPP

namespace httpflex {
    class Response
    {
        public:
        unsigned int status;
        std::string* body;
        std::map<std::string, std::string> header;
    };

    class Request
    {
        public:
        std::string method;
        std::string url;
        std::string version;
        std::map<std::string, std::string> header;
        std::string *body;

        Response Send(unsigned int status, std::string& body);
    };
}

#endif
