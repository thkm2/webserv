#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <exception>
#include <cstring>      // memset
#include <unistd.h>     // close, read, write
#include <sys/socket.h> // socket, bind, listen, accept
#include <netinet/in.h> // sockaddr_in, htons
#include <arpa/inet.h>  // inet_addr
#include <netinet/tcp.h> // TCP_NODELAY
#include <fstream>
#include <sstream>

#endif