/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientIO.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 22:35:24 by msafa             #+#    #+#             */
/*   Updated: 2026/09/29 18:58:24 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"

void handleClientDisconnect(std::vector<Client*>& connected_clients, size_t index)
{
    delete connected_clients[index];
    connected_clients.erase(connected_clients.begin() + index);
}

static void resetClientForNextRequest(Client* client)
{
    delete client->request;
    client->request = new Request();
    delete client->response;
    client->response = new Response();
    client->keep_alive = false;
    client->response_ready = false;
}

static bool finishSend(std::vector<Client*>& clients, size_t i)
{
    if(clients[i]->keep_alive)
    {
        resetClientForNextRequest(clients[i]);
        return false;
    }
    handleClientDisconnect(clients,i);
    return true;
}

void handleClientData(std::vector<Client*>& connected_clients, std::vector<struct pollfd>& fds, size_t serverCount)
{
    for(size_t i = 0; i < connected_clients.size(); i++)
    {
        if(!(fds[serverCount + i].revents & POLLIN))
            continue;
        char buffer[1024];
        ssize_t bytesReceived = recv(fds[serverCount + i].fd, buffer, sizeof(buffer) - 1,0);
        if(bytesReceived > 0)
        {
            connected_clients[i]->last_activity = time(NULL);
            std::string chunk(buffer,bytesReceived);
            connected_clients[i]->request->parse(chunk);
            processClientRequest(connected_clients[i]);
        }
        else if(bytesReceived == 0 || bytesReceived == -1)
        {
            handleClientDisconnect(connected_clients,i);
            i--;
        }
    }
}

void handleClientSend(std::vector<Client*>& connected_clients,std::vector<struct pollfd>& fds,size_t serverCount)
{
    for(size_t i = 0; i < connected_clients.size(); i++)
    {
        if(serverCount + i >= fds.size())
            break;
        if(!((fds[serverCount + i].revents & POLLOUT) && connected_clients[i]->send_buffer.length() > 0))
            continue;
        ssize_t bytesSent = send(connected_clients[i]->fd,
                                 connected_clients[i]->send_buffer.c_str(),
                                 connected_clients[i]->send_buffer.length(),0);
        if(bytesSent < 0)
        {
            handleClientDisconnect(connected_clients,i);
            i--;
            continue;
        }
        connected_clients[i]->send_buffer.erase(0,bytesSent);
        if(connected_clients[i]->send_buffer.empty())
        {
            if(finishSend(connected_clients,i))
                i--;
        }
    }
}
