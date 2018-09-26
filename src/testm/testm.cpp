#include "testm.hpp"
#include "httpflex/httpflex.hpp"
#include "httpflex/socket.hpp"

httpflex::Response RequestHandler(httpflex::Request request)
{
    std::string body;

    body = "Hello World from http server made in C++";
    return request.Send(200, body);
}

int main()
{
    httpflex::Server server;

    server.SetRequestHandler(RequestHandler);
    server.ListenOnPort(3000);
    return 0;
}
