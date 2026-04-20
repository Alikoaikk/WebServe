/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 00:32:10 by akoaik            #+#    #+#             */
/*   Updated: 2026/04/20 21:37:31 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/headers/imports.hpp"

int main(void)
{
    //vector of client structs for poll() loop
    std::vector<Client*> connected_clients;
    
    // create listening socket on port 8080
    int listenFd = Socket::createListenSocket("127.0.0.1", 8080);
    std::cout << "Server listening on 127.0.0.1:8080" << std::endl;
    

    while(true)
    {
        //create pollfd vector
        int numFds =  1 + connected_clients.size();
        std::vector<struct pollfd> fds(numFds);

        // fill the vector with the listenFd before the clients fds
        fds[0].fd = listenFd;
        fds[0].events = POLLIN;
        fds[0].revents = 0;

        //fill the clients Fds
        for(size_t i = 0; i < connected_clients.size(); i++)
        {
            fds[i + 1].fd = connected_clients[i]->fd;
            fds[i + 1].events = POLLIN;
            fds[i + 1].revents = 0;
        }
        time_t currentTime =  time(NULL);
        for(size_t i = 0; i < connected_clients.size(); i++)
        {
            if(currentTime - connected_clients[i]->last_activity > 30)
            {
                delete connected_clients[i];
                connected_clients.erase(connected_clients.begin() + i);
                i--;
            }
        }
        // call poll
        poll(&fds[0], numFds, -1);
        if(fds[0].revents & POLLIN)
        {
            struct sockaddr_in clientAddr;
            socklen_t addrLen = sizeof(clientAddr);
            int clientFd = accept(listenFd,(struct sockaddr* )&clientAddr,&addrLen);
            if(clientFd != -1)
            {
                Client* client = new Client(clientFd);
                connected_clients.push_back(client);
            }
            else
            {
                std::cerr << "accept() failed" << std::endl;
            }
        }
       for(size_t i = 0; i < connected_clients.size(); i++)
       {
        if(fds[i + 1].revents & POLLIN)
        {
            char buffer[1024];
            ssize_t bytesReceieved = recv(fds[i + 1].fd,buffer,sizeof(buffer) - 1,0);
            if(bytesReceieved > 0)
            {
                connected_clients[i]->last_activity = time(NULL);
                buffer[bytesReceieved] = '\0';
                std::string chunk(buffer);
                connected_clients[i]->recv_buffer += chunk;
                connected_clients[i]->request->parse(connected_clients[i]->recv_buffer);
                if(connected_clients[i]->request->_parseState == PARSE_COMPLETE)
                {
                    std::string httpResponse = connected_clients[i]->response->build();
                    ssize_t bytesSent = send(connected_clients[i]->fd,
                                            httpResponse.c_str(),
                                            httpResponse.length(),
                                            0);
                    if(bytesSent < 0)
                    {
                        std::cerr << "send() failed" << std::endl;
                        delete connected_clients[i];
                        connected_clients.erase(connected_clients.begin() + i);
                        i--;
                    }
                }
            }
            else if(bytesReceieved == 0 || bytesReceieved == -1)
            {
                delete connected_clients[i];
                connected_clients.erase(connected_clients.begin() + i);
                i--;
            }
        }
       }
    }
}

// int main(int argc, char **argv)
// {
//     if (argc != 2)
//     {
//         std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
//         return (1);
//     }

//     try
//     {
//         parse config(argv[1]);
//         printConfig(config);
//         // Then start the server loop using the parsed config
//     }
//     catch (std::exception &e)
//     {
//         std::cerr << "Error: " << e.what() << std::endl;
//         return (1);
//     }
//     return (0);
// }
