/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 21:15:37 by msafa             #+#    #+#             */
/*   Updated: 2026/04/12 17:52:55 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <fcntl.h>

/*
step 1: create the socket by requesting a fd from the OS to represent the network endpoint
            AF_INET : IPv4 address family
            SOCK_STREAM : TCP protocol
            0 : default protocol selection
            
step 2: configure Socket with SO_REUSADDR to prevent "address already in use"
        SOL_SOCKET : socket-level options
        SP_REUSADDR : allow port reuse immediately after server restart

step 3: prepare address structure that will be used to bind the socket 
            htons to convert port from host byte order to network byte order
            inet_pton to convert IP address string to binary format understandable by OS/network

step 4: bind socket to address so the OS will register the socket on the
        specified host and port
    
step 5: listen for connections
        128 is how many pending connections can queue up so if 128 clients
        try to connect before we accept the 129th gets refused

*/

int Socket::createListenSocket(const std::string& host, int port)
{
    struct sockaddr_in addr;
    
    int fd = socket(AF_INET, SOCK_STREAM,0);
    if(fd == -1)
        throw std::runtime_error("Socket() failed");

    int option = 1;
    if(setsockopt(fd, SOL_SOCKET,SO_REUSEADDR,&option,sizeof(option)) < 0)
    {
        close(fd);
        throw std::runtime_error("setsockopt SO_REUSEADDR failed");
    }

    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if(inet_pton(AF_INET,host.c_str(),&addr.sin_addr) <= 0)
    {
        close(fd);
        throw std::runtime_error("Invalid host: " + host);
    }

    if(bind(fd,(struct sockaddr*)&addr,sizeof(addr)) < 0)
    {
        close(fd);
        throw std::runtime_error("bind() failed");
    }
    
    if(listen(fd,128) < 0)
    {
        close(fd);
        throw std::runtime_error("listen() failed");
    }

    //make the socket non-blocking so poll() can manage it without blocking
    setNonBlocking(fd);
    return fd;
}

void Socket::setNonBlocking(int fd)
{
    int flags = fcntl(fd,F_GETFL,0);
    if(flags == -1)
        throw std::runtime_error("fcntl F_GETFL failed");
    if(fcntl(fd,F_SETFL,flags | O_NONBLOCK) == -1)
        throw std::runtime_error("fcntl F_SETFL O_NONBLOCK failed");
}

/*
fcntl() is a system call that manipulates file descriptors
        get settings on a fd
        change settings on a fd
        lock files

File Descriptor Control Panel contains flags (settings):
┌─────────────────────────────┐
│ O_NONBLOCK     [OFF] [ON] ← │  Non-blocking mode
│ O_APPEND       [OFF] [ON] ← │  Always write at end
│ O_ASYNC        [OFF] [ON] ← │  Asynchronous I/O
│ O_CLOEXEC      [OFF] [ON] ← │  Close on exec
└─────────────────────────────┘
each flag is a bit(0 or 1)

*/