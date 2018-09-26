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
        ssize_t sizeBuf;
        Header header;
        Response response;

        if (message.find("\r\n\r\n") == std::string::npos) {
            while (message.find("\r\n\r\n") == std::string::npos) {
                std::string messagePart;

                /* TODO: Check If ReceiveFromSocket return an error (if sizeBuf == -1 || sizeBuf == 0
                 * Value of 0 mean client disconnected
                 */
                sizeBuf = ReceiveFromSocket(clientfd, messagePart);
                message += messagePart;
            }
        }

        std::istringstream messageStream(message);
        std::string line;
        bool firstLine = true;

        // Parse headers
        while (std::getline(messageStream, line, '\r')) {
            if (firstLine == true) { // Ignore first line which is Request line (GET / ...)
                firstLine = false;
                continue;
            }

            if (line == "") {
                continue;
            }

            if (line.size() == 1) {
                continue;
            }

            try {
                header = ParseHeader(line);
            } catch (std::runtime_error& err) {
                #ifdef HTTPFLEX_DEBUG
                std::cout << "httpflex (2): " << err.what() << std::endl;
                #endif
                return;
            }

            // TODO: Trim header
            request.header.insert({header.name, header.value});
        }

        try {
            response = requestHandler(request);
        } catch(std::exception& err) {
            #ifdef HTTPFLEX_DEBUG
                std::cout << "httpflex: Request Handler threw an error: " << err.what();
            #endif
            return;
        } catch(...) {
            #ifdef HTTPFLEX_DEBUG
            std::cout << "httpflex: Request handler threw an error";
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
