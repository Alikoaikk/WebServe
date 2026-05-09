/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 18:15:03 by msafa             #+#    #+#             */
/*   Updated: 2026/05/09 15:28:24 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/imports.hpp"

Client::Client(int fd)
    : fd(fd),state(INBOUND_HEADER),last_activity(time(NULL)),response_ready(false)
{
    request = new Request();
    response = new Response();
}

Client::~Client()
{
    delete request;
    delete response;
}