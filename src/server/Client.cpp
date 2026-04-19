/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 18:15:03 by msafa             #+#    #+#             */
/*   Updated: 2026/04/13 18:30:10 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/classes/Client.hpp"
#include "../../includes/classes/Request.hpp"
#include "../../includes/classes/Response.hpp"
#include <ctime>

Client::Client(int fd)
    : fd(fd),state(INBOUND_HEADER),last_activity(time(NULL))
{
    request = new Request();
    response = new Response();
}

Client::~Client()
{
    delete request;
    delete response;
}