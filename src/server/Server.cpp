/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 19:30:00 by msafa             #+#    #+#             */
/*   Updated: 2026/05/03 22:36:11 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/imports.hpp"

static int createSocket()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        throw std::runtime_error("socket() failed");
    return fd;
}

static void configureSocket(int fd)
{
    int option = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0)
    {
        close(fd);
        throw std::runtime_error("setsockopt SO_REUSEADDR failed");
    }
}

static void bindSocket(int fd, const std::string& host, int port)
{
    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0)
    {
        close(fd);
        throw std::runtime_error("Invalid host: " + host);
    }
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        close(fd);
        throw std::runtime_error("bind() failed");
    }
}

static void listenSocket(int fd)
{
    if (listen(fd, 128) < 0)
    {
        close(fd);
        throw std::runtime_error("listen() failed");
    }
}

Server::Server()
    : listenFd(-1),config()
{}

void Server::createListenSocket(const std::string& host, int port)
{
    this->listenFd = createSocket();
    configureSocket(this->listenFd);
    bindSocket(this->listenFd, host, port);
    listenSocket(this->listenFd);
    setNonBlocking(this->listenFd);
}

Server::Server(const Server& other)
    : listenFd(-1), config(other.config)
{
    if (other.listenFd != -1)
    {
        try
        {
            createListenSocket(config.host, config.port);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Copy constructor failed: " << e.what() << std::endl;
            throw;
        }
    }
}

Server::Server(const parse::serConfig& cfg)
    : listenFd(-1), config(cfg)
{
    try
    {
        createListenSocket(cfg.host, cfg.port);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to create server on " << cfg.host << ":"
                  << cfg.port << " - " << e.what() << std::endl;
        throw;
    }
}

Server& Server::operator=(const Server& other)
{
    if (this != &other)
    {
        if (listenFd != -1)
        {
            close(listenFd);
            listenFd = -1;
        }
        config = other.config;
        if (other.listenFd != -1)
        {
            try
            {
                createListenSocket(config.host, config.port);
            }
            catch (const std::exception& e)
            {
                std::cerr << "Assignment operator failed: " << e.what() << std::endl;
                throw;
            }
        }
    }
    return *this;
}

Server::~Server()
{
    if (listenFd != -1)
    {
        close(listenFd);
        listenFd = -1;
    }
}

int Server::getListenFd() const
{
    return listenFd;
}

const parse::serConfig& Server::getConfig() const
{
    return config;
}

static void handleAcceptError(int errorCode)
{
    if (errorCode == EAGAIN || errorCode == EWOULDBLOCK)
        return;
    if (errorCode == EINTR)
        return;
    if (errorCode == EMFILE)
    {
        std::cerr << "accept() failed: Process file descriptor limit reached" << std::endl;
        return;
    }
    if (errorCode == EBADF)
    {
        std::cerr << "accept() failed: Invalid listening socket (BUG!)" << std::endl;
        return;
    }
    std::cerr << "accept() failed: " << strerror(errorCode) << std::endl;
}

void Server::acceptNewClient(std::vector<Client*>& connected_clients)
{
    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    int clientFd = accept(listenFd, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientFd != -1)
    {
        try
        {
            Client* client = new Client(clientFd);
            connected_clients.push_back(client);
        }
        catch (const std::exception& e)
        {
            close(clientFd);
            std::cerr << "Failed to create client: " << e.what() << std::endl;
        }
        return;
    }
    if (errno == EINTR)
    {
        acceptNewClient(connected_clients);
        return;
    }
    handleAcceptError(errno);
}

void Server::setNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        throw std::runtime_error("fcntl F_GETFL failed");
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
        throw std::runtime_error("fcntl F_SETFL O_NONBLOCK failed");
}
