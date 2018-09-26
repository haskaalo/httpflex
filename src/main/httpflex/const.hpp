#ifndef HTTPFLEX_CONST_HPP
#define HTTPFLEX_CONST_HPP

//
// Config
//
#define HTTPFLEX_BACKLOG 10 // How many pending connection queue can hold
#define HTTPFLEX_MAX_BUFFER_SIZE 3000
#define HTTPFLEX_SEND_BUFFER_SIZE(bufleft) ((bufleft > HTTPFLEX_MAX_BUFFER_SIZE) ? HTTPFLEX_MAX_BUFFER_SIZE : bufleft)

#define HTTPFLEX_DEBUG // Print debug messages to console

#endif
