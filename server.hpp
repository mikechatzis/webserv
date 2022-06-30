#ifndef server_hpp
#define server_hpp

#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <fcntl.h> // For fcntl
#include <netdb.h> // For addrinfo
#include <arpa/inet.h> // For inet_ntop
#include <map>

#define PORT "4242"

#endif
