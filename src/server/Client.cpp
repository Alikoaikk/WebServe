/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 18:15:03 by msafa             #+#    #+#             */
/*   Updated: 2026/10/01 23:17:50 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"

Client::Client(int fd)
    :   fd(fd),
        request(new Request()),
        response(new Response()),
        last_activity(time(NULL)),
        response_ready(false) ,
        keep_alive(false),
        serverConfig(NULL),
        cgi(NULL)
{}

Client::~Client()
{
    if(fd != -1)
        close(fd);
    delete request;
    delete response;
    cgiCleanup(cgi);
    delete cgi ;
}
