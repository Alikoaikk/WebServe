/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 20:17:49 by msafa             #+#    #+#             */
/*   Updated: 2026/10/02 23:47:38 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"

int createSocket()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        throw std::runtime_error("socket() failed");
    return fd;
}

void configureSocket(int fd)
{
    int option = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0)
        throw std::runtime_error("setsockopt SO_REUSEADDR failed");
}

void bindSocket(int fd, const std::string& host, int port)
{
    struct addrinfo hints;
    struct addrinfo* res = NULL;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    std::ostringstream oss;
    oss << port;
    if(getaddrinfo(host.c_str(),oss.str().c_str(), &hints, &res) != 0)
        throw std::runtime_error("Invalid host: " + host);
    int ret = bind(fd, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
    if(ret < 0)
        throw std::runtime_error("bind() failed");
}

void listenSocket(int fd)
{
    if (listen(fd, 128) < 0)
        throw std::runtime_error("listen() failed");
}

void setNonBlocking(int fd)
{
    if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("fcntl F_SETFL O_NONBLOCK failed");
}
