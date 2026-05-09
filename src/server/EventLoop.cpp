/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 22:35:24 by msafa             #+#    #+#             */
/*   Updated: 2026/05/09 19:08:21 by msafa            ###   ########.fr       */
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
        if(connected_clients[i]->response_ready && connected_clients[i]->send_buffer.length() > 0)
            fds[servers.size() + i].events = POLLIN  | POLLOUT;
        fds[servers.size() + i].revents = 0;
    }
}

void handleClientDisconnect(std::vector<Client*>& connected_clients, size_t index)
{
    delete connected_clients[index];
    connected_clients.erase(connected_clients.begin() + index);
}

void handleClientSend(std::vector<Client*>& connected_clients,std::vector<struct pollfd>& fds,size_t serverCount)
{
    for(size_t i = 0; i < connected_clients.size(); i++)
    {
        if(serverCount + i >= fds.size())
            break;
        if((fds[serverCount + i].revents & POLLOUT) && connected_clients[i]->send_buffer.length() > 0)
        {
            ssize_t bytesSent = send(connected_clients[i]->fd,
                                     connected_clients[i]->send_buffer.c_str(),
                                     connected_clients[i]->send_buffer.length(),
                                     0);
            if(bytesSent > 0)
            {
                connected_clients[i]->send_buffer.erase(0,bytesSent);
                if(connected_clients[i]->send_buffer.length() == 0)
                {
                    connected_clients[i]->response_ready = false;
                    handleClientDisconnect(connected_clients,i);
                    i--;
                }
            }
            else if(bytesSent < 0)
            {
                handleClientDisconnect(connected_clients, i);
                i--;
            }
        }
    }
}

static const parse::locConfig* findLocation(const parse::serConfig& config, const std::string& uri)
{
    const parse::locConfig* winner = NULL;
    size_t winnerLen = 0;
    for(size_t i = 0; i < config.locations.size(); i++)
    {
        const std::string& locPath = config.locations[i].path;
        if(uri.find(locPath) == 0 && locPath.length() > winnerLen)
        {
            winner = &config.locations[i];
            winnerLen = locPath.length();
        }
    }
    return winner;
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
                    const parse::locConfig* loc = findLocation(*connected_clients[i]->serverConfig,connected_clients[i]->request->_uri);
                    if(loc == NULL)
                    {
                        connected_clients[i]->response->setStatusCode(404);
                        connected_clients[i]->response->setHeader("Content-Type","text/html");
                        connected_clients[i]->response->setBody("<html><body><h1>404 Not Found</h1></body></html>");
                    }
                    else
                    {
                        const std::vector<std::string>& methods = loc->methods;
                        bool methodAllowed = false;
                        for(size_t j = 0; j < methods.size(); j++)
                        {
                            if(methods[j] == connected_clients[i]->request->_method)
                            {
                                methodAllowed = true;
                                break;
                            }
                        }
                        if(!methodAllowed)
                        {
                            connected_clients[i]->response->setStatusCode(405);
                            connected_clients[i]->response->setHeader("Content-Type","text/html");
                            connected_clients[i]->response->setBody("<html><body><h1>405 Method Not Allowed</h1></body></html>");          
                        }
                        else
                        {
                            connected_clients[i]->response->setStatusCode(200);
                            connected_clients[i]->response->setHeader("Content-Type","text/plain");
                            connected_clients[i]->response->setBody("OK");
                        }
                    }
                    connected_clients[i]->send_buffer = connected_clients[i]->response->build();
                    connected_clients[i]->response_ready = true;
                }
                else if(connected_clients[i]->request->_parseState == PARSE_ERROR)
                {
                    connected_clients[i]->response->setStatusCode(400);
                    connected_clients[i]->response->setHeader("Content-Type","text/html");
                    connected_clients[i]->response->setBody("<html><body><h1>400 Bad Request</h1></body></html>");
                    connected_clients[i]->send_buffer = connected_clients[i]->response->build();
                    connected_clients[i]->response_ready = true;
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
        handleClientSend(connected_clients,fds,servers.size());
    }
}
