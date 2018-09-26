#include "httpflex/httpflex.hpp"

#include <iostream>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <thread>
#include <sstream>

#include "httpflex/const.hpp"
#include "httpflex/utils.hpp"
#include "httpflex/context.hpp"
#include "httpflex/socket.hpp"

namespace httpflex {
    void Server::SetRequestHandler(std::function<Response (Request)> func)
    {
        requestHandler = func;
    }

    void Server::ListenOnPort(unsigned int port)
    {
        struct addrinfo hints, *servinfo, *p;
        int addrStatus;
        int sockopt;
        int yes = 1;

        if (port < 1024 || port > 65535) {
            throw std::runtime_error("Port exceeded maximum number");
        }
        std::string portStr = std::to_string(port);
        char const *portChar = portStr.c_str();

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        if ((addrStatus = getaddrinfo(NULL, portChar, &hints, &servinfo)) != 0) {
            throw std::runtime_error(gai_strerror(addrStatus));
        }

        // bind to the first we can find
        for (p = servinfo; p != NULL; p = p->ai_next) {
            if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                throw std::runtime_error(strerror(errno));
            }

            if ((sockopt = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) == -1) {
                throw std::runtime_error(strerror(errno));
            }

            if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                throw std::runtime_error(strerror(errno));
            }
        }

        freeaddrinfo(servinfo);

        if (listen(sockfd, HTTPFLEX_BACKLOG) == -1) {
            throw std::runtime_error(strerror(errno));
        }

        AcceptConnections();
    }

    void Server::AcceptConnections()
    {
        struct sockaddr_storage their_addr;
        socklen_t sin_size;
        int clientfd;

        for (;;) {
            sin_size = sizeof their_addr;
            clientfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
            if (clientfd == -1) {
                #ifdef HTTPFLEX_DEBUG
                    std::cout << "httpflex: Failed to accept a connection" << std::endl;
                #endif
                continue;
            }
            std::thread (&Server::HandleRequest, this, clientfd).detach();
        }
    }

    void Server::HandleRequest(int clientfd)
    {
        ssize_t msgSize;
        std::string fullMessage, requestLine;
        std::vector<std::string> splitRequestLine;
        Request request;

        while (fullMessage.find("\r\n") == std::string::npos) {
            std::string requestBuf;

            msgSize = ReceiveFromSocket(clientfd, requestBuf);
            if (msgSize == -1) {
                #ifdef HTTPFLEX_DEBUG
                std::cout << "httpflex: Failed to receive request line" << std::endl;
                #endif
                return;
            } else if (msgSize == 0) {
                #ifdef HTTPFLEX_DEBUG
                std::cout << "httpflex: " << HTTPFLEX_CLIENT_DISCONNECTED_ERROR << std::endl;
                #endif
                return;
            }

            fullMessage += requestBuf;
        }

        std::istringstream messageStream(fullMessage);
        std::getline(messageStream, requestLine, '\r');

        if (requestLine == "") {
            #ifdef HTTPFLEX_DEBUG
            std::cout << "httpflex: Empted request line";
            #endif
            return;
        }

        splitRequestLine = Split(requestLine, ' ');
        if (splitRequestLine.size() != 3) {
            #ifdef HTTPFLEX_DEBUG
            std::cout << "httpflex: Invalid Request line size." <<std::endl << "Length: " << splitRequestLine.size() << std::endl;
            #endif
            return;
        }

        request.method = splitRequestLine[0];
        request.url = splitRequestLine[1];
        request.version = splitRequestLine[2];

        if (request.method == "GET") {
            HandleMethodGet(clientfd, request, fullMessage);
        } else {
            #ifdef HTTPFLEX_DEBUG
            std::cout << "httpflex: Invalid Request Method." << std::endl << "Received: " << request.method << std::endl;
            #endif
            return;
        }

        close(clientfd);
    }

    void Server::SendMessage(int clientfd, Response& response)
    {
        std::string statusLineHeader = "HTTP/1.1 " + std::to_string(response.status) + " " + ReasonPhrase[response.status] + "\r\n";

        for (const auto& keyval : response.header) {
            statusLineHeader += keyval.first + ": " + keyval.second + "\r\n";
        }
        statusLineHeader += "\r\n";

        try {
            SendToSocket(clientfd, statusLineHeader);
            SendToSocket(clientfd, *response.body);
        } catch(std::runtime_error& err) {
            throw;
        }
    }
}
