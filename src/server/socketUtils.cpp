/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 20:17:49 by msafa             #+#    #+#             */
/*   Updated: 2026/09/29 20:17:50 by msafa            ###   ########.fr       */
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
    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0)
        throw std::runtime_error("Invalid host: " + host);
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("bind() failed");
}

void listenSocket(int fd)
{
    if (listen(fd, 128) < 0)
        throw std::runtime_error("listen() failed");
}

void setNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        throw std::runtime_error("fcntl F_GETFL failed");
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
        throw std::runtime_error("fcntl F_SETFL O_NONBLOCK failed");
}
