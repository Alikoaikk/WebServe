/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 18:15:03 by msafa             #+#    #+#             */
/*   Updated: 2026/07/26 18:04:56 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"

Client::Client(int fd)
    : fd(fd),state(INBOUND_HEADER),last_activity(time(NULL)),
        response_ready(false) , keep_alive(false), serverConfig(NULL)
{
    request = new Request();
    response = new Response();
}

Client::~Client()
{
    if(fd != -1)
        close(fd);
    delete request;
    delete response;
}