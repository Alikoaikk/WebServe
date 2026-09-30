/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 18:31:56 by akoaik            #+#    #+#             */
/*   Updated: 2026/09/30 22:56:22 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <ctime>
#include <sys/types.h>

typedef struct cgi
{
    pid_t pid;
    int inFd;
    int outFd;
    std::string body;
    size_t bodySent;
    std::string output;
    time_t startTime;
    bool done;
    cgi()
        :   pid(-1),
            inFd(-1),
            outFd(-1),
            startTime(0),
            done(false)
    {}
} cgi_process ;

struct Client;

void cgiFinishResponse(Client* client);

#endif
