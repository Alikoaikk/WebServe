/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiIO.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/01 21:21:44 by akoaik            #+#    #+#             */
/*   Updated: 2026/10/04 21:39:52 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"
#include <sys/wait.h>

static void cgiWriteBody(cgi_process *cgi)
{
    size_t left ;

    left = cgi->body.size() - cgi->bodySent ;
    
    if (left > 0)
    {
        ssize_t n = write(cgi->inFd, cgi->body.c_str() + cgi->bodySent, left);
        if (n > 0)
            cgi->bodySent += n;
        else
            cgi->bodySent = cgi->body.size();
    }
    if(cgi->bodySent >= cgi->body.size())
    {
        close(cgi->inFd);
        cgi->inFd = -1;
    }
}

static bool cgiReadOutput(cgi_process* cgi)
{
    char buffer[4096];
    ssize_t n = read(cgi->outFd, buffer, sizeof(buffer));
    if(n > 0)
    {
        cgi->output.append(buffer, n);
        return false;
   }
   close(cgi->outFd);
   cgi->outFd = -1;
   return true;
}

static void cgiComplete(Client* client)
{
    cgi_process* cgi = client->cgi;
    int status = 0;

    if(cgi->inFd != -1)
    {
        close(cgi->inFd);
        cgi->inFd = -1;
    }
    if(waitpid(cgi->pid, &status, WNOHANG) == 0)
    {
        kill(cgi->pid, SIGKILL);
        waitpid(cgi->pid, &status, 0);
    }
    cgi->pid = -1;
    if(WIFEXITED(status) && WEXITSTATUS(status) != 0)
        buildErrorResponse(client,502);
    else
        cgiFinishResponse(client);
    delete client->cgi;
    client->cgi = NULL;
}

void handleCgiIO(std::vector<Client*>& connected_clients, std::vector<struct pollfd>& fds)
{
    for(size_t i = 0; i < connected_clients.size(); i++)
    {
        Client* client = connected_clients[i];
        if(client->cgi == NULL)
            continue;
        if(client->cgi->inFd != -1
            && (getRevents(fds, client->cgi->inFd) & (POLLOUT | POLLERR | POLLHUP)))
            cgiWriteBody(client->cgi);
        if(client->cgi->outFd != -1
            && (getRevents(fds, client->cgi->outFd) & (POLLIN | POLLERR | POLLHUP)))
        {
            if(cgiReadOutput(client->cgi))
                cgiComplete(client);
        }
    }
}

void cgiCleanup(cgi_process* cgi)
{
    if(cgi == NULL)
        return;
    if(cgi->inFd != -1)
    {
        close(cgi->inFd);
        cgi->inFd = -1;
    }
    if(cgi->outFd != -1)
    {
        close(cgi->outFd);
        cgi->outFd = -1;
    }
    if(cgi->pid != -1)
    {
        kill(cgi->pid, SIGKILL);
        waitpid(cgi->pid, NULL, 0);
        cgi->pid = -1;
    }
}