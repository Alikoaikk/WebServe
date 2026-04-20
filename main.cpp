/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 00:32:10 by akoaik            #+#    #+#             */
/*   Updated: 2026/04/20 21:40:00 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/headers/imports.hpp"

static void checkClientTimeouts(std::vector<Client*>& connected_clients)
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

static void buildPollArray(std::vector<struct pollfd>& fds, int listenFd, std::vector<Client*>& connected_clients)
{
    fds.clear();
    fds.resize(1 + connected_clients.size());

    fds[0].fd = listenFd;
    fds[0].events = POLLIN;
    fds[0].revents = 0;

    for (size_t i = 0; i < connected_clients.size(); i++)
    {
        fds[i + 1].fd = connected_clients[i]->fd;
        fds[i + 1].events = POLLIN;
        fds[i + 1].revents = 0;
    }
}

static void handleNewClient(int listenFd, std::vector<Client*>& connected_clients)
{
    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    int clientFd = accept(listenFd, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientFd != -1)
    {
        Client* client = new Client(clientFd);
        connected_clients.push_back(client);
    }
    else
    {
        std::cerr << "accept() failed" << std::endl;
    }
}

static void handleClientResponse(Client* client)
{
    if (client->request->_parseState == PARSE_COMPLETE)
    {
        client->response->setStatusCode(200);
        client->response->setHeader("Content-Type", "text/plain");
        client->response->setBody("OK");
        std::string httpResponse = client->response->build();

        ssize_t bytesSent = send(client->fd,
                                httpResponse.c_str(),
                                httpResponse.length(),
                                0);
        if (bytesSent < 0)
        {
            std::cerr << "send() failed" << std::endl;
        }
    }
}

static void handleClientDisconnect(std::vector<Client*>& connected_clients, size_t index)
{
    delete connected_clients[index];
    connected_clients.erase(connected_clients.begin() + index);
}

static void handleClientData(std::vector<Client*>& connected_clients, std::vector<struct pollfd>& fds)
{
    for (size_t i = 0; i < connected_clients.size(); i++)
    {
        if (fds[i + 1].revents & POLLIN)
        {
            char buffer[1024];
            ssize_t bytesReceieved = recv(fds[i + 1].fd, buffer, sizeof(buffer) - 1, 0);

            if (bytesReceieved > 0)
            {
                connected_clients[i]->last_activity = time(NULL);
                buffer[bytesReceieved] = '\0';
                std::string chunk(buffer);
                connected_clients[i]->recv_buffer += chunk;
                connected_clients[i]->request->parse(connected_clients[i]->recv_buffer);
                handleClientResponse(connected_clients[i]);
            }
            else if (bytesReceieved == 0 || bytesReceieved == -1)
            {
                handleClientDisconnect(connected_clients, i);
                i--;
            }
        }
    }
}

int main(void)
{
    std::vector<Client*> connected_clients;
    std::vector<struct pollfd> fds;
    int listenFd = Socket::createListenSocket("127.0.0.1", 8080);
    std::cout << "Server listening on 127.0.0.1:8080" << std::endl;

    while (true)
    {
        checkClientTimeouts(connected_clients);

        buildPollArray(fds, listenFd, connected_clients);
        poll(&fds[0], fds.size(), -1);

        if (fds[0].revents & POLLIN)
            handleNewClient(listenFd, connected_clients);

        handleClientData(connected_clients, fds);
    }

    return (0);
}
