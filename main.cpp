/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 00:32:10 by akoaik            #+#    #+#             */
/*   Updated: 2026/04/18 18:48:24 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Socket.hpp"
#include "http/Request.hpp"
#include "http/Response.hpp"
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

int main(void)
{
    // create listening socket on port 8080
    int listenFd = Socket::createListenSocket("127.0.0.1", 8080);
    std::cout << "Server listening on 127.0.0.1:8080" << std::endl;

    while(true)
    {
        // wait for one client connection
        struct sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);
        int clientFd = accept(listenFd, (struct sockaddr*)&clientAddr, &addrLen);

        if(clientFd == -1)
            break;

        // receive data from client
        char buffer[1024];
        int bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
        buffer[bytesRead] = '\0';

        Request request;
        std::string chunk(buffer);
        request.parse(chunk);

        std::cout << "Method: " << request._method << std::endl;
        std::cout << "URI: " << request._uri << std::endl;
        std::cout << "Body: " << request._body << std::endl;

        // Create Response
        Response response;
        response.setStatusCode(200);
        response.setHeader("Content-Type", "text/plain");
        response.setBody("Hello from WebServ!");

        // Build and send response
        std::string httpResponse = response.build();
        send(clientFd, httpResponse.c_str(), httpResponse.length(), 0);

        close(clientFd);
    }

    close(listenFd);
    return(0);
}