/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 18:01:41 by msafa             #+#    #+#             */
/*   Updated: 2026/09/30 02:55:35 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <ctime>
#include "Parsing.hpp"

class Request;
class Response;

struct Client
{
    int fd; //which socket(identify the client)
    std::string send_buffer; // response bytes to send
    Request* request;  // the object that will parse the request
    Response* response; // the object that will build the response
    time_t last_activity; //timestamp for timeout detection
    bool response_ready; // flag: response is built and waiting to send
    bool keep_alive; //flag: reuse this connection after the current response is sent
    const parse::serConfig* serverConfig; // points to the server's config that the client is connected to
    cgi_process *cgi ;  // ali : pointer to connect to each client runing CGI

    Client(int fd);
    ~Client();
};

#endif
