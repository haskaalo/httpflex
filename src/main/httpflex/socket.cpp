#include "httpflex/socket.hpp"

#include <iostream>
#include <string.h>
#include <sys/socket.h>

#include "const.hpp"

namespace httpflex {
    ssize_t ReceiveFromSocket(int fd, std::string& msg)
    {
        char msgReceived[HTTPFLEX_MAX_BUFFER_SIZE];
        ssize_t msgSize;

        memset(&msgReceived, 0, sizeof(msgReceived));
        msgSize = recv(fd, &msgReceived, sizeof(msgReceived), 0);
        msg = std::string(msgReceived);

        return msgSize;
    }

    void SendToSocket(int fd, std::string& msg)
    {
        char* bufToSend;
        std::size_t bufSize;
        ssize_t prevByteSent = 0;
        int totalByteSent = 0;

        while (totalByteSent < msg.size()) {
            bufToSend = new char[totalByteSent+HTTPFLEX_SEND_BUFFER_SIZE((msg.size() - totalByteSent) + 1)];
            bufSize = msg.copy(bufToSend, HTTPFLEX_SEND_BUFFER_SIZE(msg.size() - totalByteSent), totalByteSent);
            bufToSend[bufSize] = '\0';

            prevByteSent = send(fd, bufToSend, strlen(bufToSend), 0);

            if (prevByteSent == -1) {
                delete [] bufToSend;
                throw std::runtime_error(strerror(errno));
            } else if (prevByteSent == 0) {
                delete [] bufToSend;
                throw std::runtime_error(HTTPFLEX_CLIENT_DISCONNECTED_ERROR);
            }
            totalByteSent += prevByteSent;
        }

        delete [] bufToSend;
    }
}
