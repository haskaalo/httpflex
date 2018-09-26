#ifndef HTTPFLEX_SOCKET_HPP
#define HTTPFLEX_SOCKET_HPP

#include <iostream>
#include <vector>
#include <sys/socket.h>

#define HTTPFLEX_CLIENT_DISCONNECTED_ERROR "The client has closed the connection earlier"

namespace httpflex {
    ssize_t ReceiveFromSocket(int fd, std::string& msg);
    void SendToSocket(int fd, std::string& msg);
}

#endif
