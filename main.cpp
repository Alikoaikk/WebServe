/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 00:32:10 by akoaik            #+#    #+#             */
/*   Updated: 2026/04/19 21:03:32 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/classes/Socket.hpp"
#include "includes/classes/Request.hpp"
#include "includes/classes/Response.hpp"
#include "includes/classes/parsing.hpp"
#include "includes/headers/imports.hpp"
#include "includes/classes/Client.hpp"
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <vector>

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

        // call poll
        int pollResult = poll(&fds[0], numFds, -1);
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
