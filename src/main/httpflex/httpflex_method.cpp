#include "httpflex/httpflex.hpp"

#include <iostream>
#include <sys/socket.h>
#include <sstream>
#include <map>
#include <string.h>

#include "httpflex/const.hpp"
#include "httpflex/utils.hpp"
#include "httpflex/socket.hpp"

namespace httpflex {
    void Server::HandleMethodGet(int clientfd, Request& request, std::string& message)
    {
        Header header;
        Response response;

        try {
            response = requestHandler(request);
        } catch(std::exception& err) {
            #ifdef HTTPFLEX_DEBUG
            std::cout << "httpflex: Request Handler threw an error: " << err.what() << std::endl;
            #endif
            return;
        } catch(...) {
            #ifdef HTTPFLEX_DEBUG
            std::cout << "httpflex: Request handler threw an undefined error" << std::endl;
            #endif
            return;
        }

        try {
            SendMessage(clientfd, response);
            delete response.body;
        } catch(std::exception& err) {
            #ifdef HTTPFLEX_DEBUG
            std::cout << "httpflex: Sending response has ended up as a error: " << std::endl << err.what() << std::endl;
            #endif
            delete response.body;
            return;
        }
    }
}
