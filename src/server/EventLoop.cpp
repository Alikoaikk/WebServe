/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 22:35:24 by msafa             #+#    #+#             */
/*   Updated: 2026/10/02 00:20:41 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"

static void checkClientTimeouts(std::vector<Client*>& connected_clients)
{
    time_t currentTime = time(NULL);
    for (size_t i = 0; i < connected_clients.size(); i++)
    {
        if (currentTime - connected_clients[i]->last_activity > 75)
        {
            handleClientDisconnect(connected_clients, i);
            i--;
        }
    }
}

static void addPollFd(std::vector<struct pollfd>& fds, int fd, short int events)
{
    struct pollfd p;
    p.fd = fd;
    p.events = events;
    p.revents = 0;
    fds.push_back(p);
}

static void buildPollArray(std::vector<struct pollfd>& fds, std::vector<Server*>& servers, std::vector<Client*>& connected_clients)
{
    fds.clear();
    fds.resize(servers.size() + connected_clients.size());

    for(size_t i = 0; i < servers.size(); i++)
    {
        fds[i].fd = servers[i]->getListenFd();
        fds[i].events = POLLIN;
        fds[i].revents = 0;
    }

    for (size_t i = 0; i < connected_clients.size(); i++)
    {
        fds[servers.size() + i].fd = connected_clients[i]->fd;
        fds[servers.size() + i].events = POLLIN;
        if(connected_clients[i]->response_ready && connected_clients[i]->send_buffer.length() > 0)
            fds[servers.size() + i].events = POLLIN  | POLLOUT;
        fds[servers.size() + i].revents = 0;
    }

    for (size_t i = 0; i < connected_clients.size(); i++)
    {
        cgi_process* cgi = connected_clients[i]->cgi;
        if (cgi == NULL)
            continue;
        if (cgi->inFd != -1)
            addPollFd(fds, cgi->inFd, POLLOUT);
        if (cgi->outFd != -1)
            addPollFd(fds, cgi->outFd, POLLIN);
    }
}

static void acceptNewClients(std::vector<Server*>& servers, std::vector<struct pollfd>& fds, std::vector<Client*>& connected_clients)
{
    for (size_t i = 0; i < servers.size(); i++)
    {
        if (fds[i].revents & POLLIN)
            servers[i]->acceptNewClient(connected_clients);
    }
}

static void checkCgiTimeouts(std::vector<Client*>& connected_clients)
{
    time_t currentTime = time(NULL);
    for (size_t i = 0; i < connected_clients.size(); i++)
    {
        Client* client = connected_clients[i];
        if (client->cgi == NULL)
            continue;
        if (currentTime - client->cgi->startTime > 10)
        {
            cgiCleanup(client->cgi);
            delete client->cgi;
            client->cgi = NULL;
            buildErrorResponse(client, 504);
        }
    }
}

void runEventLoop(std::vector<Server*>& servers, std::vector<Client*>& connected_clients)
{
    std::vector<struct pollfd> fds;

    while (true)
    {
        checkCgiTimeouts(connected_clients);
        checkClientTimeouts(connected_clients);
        buildPollArray(fds, servers, connected_clients);
        poll(&fds[0], fds.size(), 5000);
        handleCgiIO(connected_clients, fds);
        acceptNewClients(servers, fds, connected_clients);
        handleClientData(connected_clients, fds);
        handleClientSend(connected_clients, fds);
    }
}
