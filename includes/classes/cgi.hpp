/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 18:31:56 by akoaik            #+#    #+#             */
/*   Updated: 2026/09/26 21:41:02 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_hpp
#define CGI_HPP

#include <string>
#include <ctime>
#include <sys/types.h>

typedef struct cgi
{
    pid_t pid;
    int intFd;
    int outFd;
    std::string body;
    size_t bodySent;
    std::string output;
    time_t startTime;
    bool done;

    cgi_process()
        : pid(-1), inFd(-1), outFd(-1), startTime(0), done(false)
    {}
} cgi_process ;





#endif
