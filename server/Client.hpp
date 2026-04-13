/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 18:01:41 by msafa             #+#    #+#             */
/*   Updated: 2026/04/13 18:22:32 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <ctime>

enum ClientState
{
    INBOUND_HEADER,
    INBOUND_BODY,
    ROUTING,
    OUTBOUND_RESPONSE,
    DONE
};

class Request;
class Response;

struct Client
{
    int fd; //which socket(identify the client)
    ClientState state;  // what phase currently..
    std::string recv_buffer; // raw bytes received
    std::string send_buffer; // response bytes to send
    Request* request;   // the object that will parse the request
    Response* response; // the object that will build the response
    time_t last_activity; //timestamp for timeout detection
    Client(int fd);
    ~Client();
};

#endif