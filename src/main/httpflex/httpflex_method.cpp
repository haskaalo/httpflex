#include "httpflex/httpflex.hpp"

#include <iostream>
#include <sys/socket.h>
#include <sstream>
#include <map>
#include <string.h>

#include "httpflex/const.hpp"
#include "httpflex/utils.hpp"

namespace httpflex {
    void Server::HandleMethodGet(int clientfd, Request& request, std::string& message)
    {
        ssize_t sizeBuf;
        char buf[HTTPFLEX_MAX_BUFFER_SIZE];
        Header header;
        Response response;

        if (message.substr(message.size() - 4, message.size()) != "\r\n\r\n") {
            for (;;) {
                std::lock_guard<std::mutex> lock(mtx);
                memset(&buf, 0, sizeof(buf));
                // TODO: Check If recv return an error (if sizeBuf = -1)
                sizeBuf = recv(clientfd, &buf, HTTPFLEX_MAX_BUFFER_SIZE, 0);
                message += buf;
                if (message.substr(message.size() - 4, message.size()) == "\r\n\r\n") {
                    break;
                }
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
                    std::cout << "httpflex: Invalid message format (header 2)" << std::endl;
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
