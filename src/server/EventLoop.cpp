/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 22:35:24 by msafa             #+#    #+#             */
/*   Updated: 2026/05/03 22:39:32 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/imports.hpp"

void checkClientTimeouts(std::vector<Client*>& connected_clients)
{
    time_t currentTime = time(NULL);
    for (size_t i = 0; i < connected_clients.size(); i++)
    {
        if (currentTime - connected_clients[i]->last_activity > 30)
        {
            delete connected_clients[i];
            connected_clients.erase(connected_clients.begin() + i);
            i--;
        }
    }
}

void buildPollArray(std::vector<struct pollfd>& fds, std::vector<Server*>& servers, std::vector<Client*>& connected_clients)
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
        fds[servers.size() + i].revents = 0;
    }
}

void handleClientDisconnect(std::vector<Client*>& connected_clients, size_t index)
{
    delete connected_clients[index];
    connected_clients.erase(connected_clients.begin() + index);
}

void handleClientData(std::vector<Client*>& connected_clients, std::vector<struct pollfd>& fds, size_t serverCount)
{
    for (size_t i = 0; i < connected_clients.size(); i++)
    {
        if (fds[serverCount + i].revents & POLLIN)
        {
            char buffer[1024];
            ssize_t bytesReceieved = recv(fds[serverCount + i].fd, buffer, sizeof(buffer) - 1, 0);

            if (bytesReceieved > 0)
            {
                connected_clients[i]->last_activity = time(NULL);
                buffer[bytesReceieved] = '\0';
                std::string chunk(buffer);
                connected_clients[i]->recv_buffer += chunk;
                connected_clients[i]->request->parse(connected_clients[i]->recv_buffer);
                if (connected_clients[i]->request->_parseState == PARSE_COMPLETE)
                {
                    connected_clients[i]->response->setStatusCode(200);
                    connected_clients[i]->response->setHeader("Content-Type", "text/plain");
                    connected_clients[i]->response->setBody("OK");
                    std::string httpResponse = connected_clients[i]->response->build();

                    ssize_t bytesSent = send(connected_clients[i]->fd,
                                            httpResponse.c_str(),
                                            httpResponse.length(),
                                            0);
                    if (bytesSent < 0)
                    {
                        std::cerr << "send() failed" << std::endl;
                    }
                }
            }
            else if (bytesReceieved == 0 || bytesReceieved == -1)
            {
                handleClientDisconnect(connected_clients, i);
                i--;
            }
        }
    }
}

void runEventLoop(std::vector<Server*>& servers, std::vector<Client*>& connected_clients)
{
    std::vector<struct pollfd> fds;

    while (true)
    {
        checkClientTimeouts(connected_clients);
        buildPollArray(fds, servers, connected_clients);
        poll(&fds[0], fds.size(), 5000);

        for(size_t i = 0; i < servers.size(); i++)
        {
            if (fds[i].revents & POLLIN)
                servers[i]->acceptNewClient(connected_clients);
        }

        handleClientData(connected_clients, fds, servers.size());
    }
}
