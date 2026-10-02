/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 19:30:00 by msafa             #+#    #+#             */
/*   Updated: 2026/10/02 23:08:44 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"

Server::Server()
    : listenFd(-1),config()
{}

void Server::createListenSocket(const std::string& host, int port)
{
    this->listenFd = createSocket();
    try
    {
        configureSocket(this->listenFd);
        bindSocket(this->listenFd, host, port);
        listenSocket(this->listenFd);
        setNonBlocking(this->listenFd);
    }
    catch (...)
    {
        close(this->listenFd);
        this->listenFd = -1;
        throw;
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

void Server::acceptNewClient(std::vector<Client*>& connected_clients)
{
    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    int clientFd = accept(listenFd, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientFd == -1)
        return;
    try
    {
        setNonBlocking(clientFd);
        Client* client = new Client(clientFd);
        client->serverConfig = &this->config;
        connected_clients.push_back(client);
    }
    catch (const std::exception& e)
    {
        close(clientFd);
        std::cerr << "Failed to create client: " << e.what() << std::endl;
    }
}
