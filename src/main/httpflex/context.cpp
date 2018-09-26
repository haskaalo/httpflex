#include "context.hpp"

namespace httpflex {
    Response Request::Send(unsigned int status, std::string& body)
    {
        Response response;
        response.body = new std::string;
        response.status = status;
        response.header.insert({"Content-Type", "text/plain"});
        *response.body = body;

        return response;
    }
}
