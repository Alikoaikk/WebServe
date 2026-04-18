/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 18:33:24 by msafa             #+#    #+#             */
/*   Updated: 2026/04/18 20:29:28 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>

class Response
{
    private:
        std::string _version;
        int _statusCode;
        std::string _statusMessage;
        std::map<std::string, std::string> _header;
        std::string _body;
    public:
        Response();
        ~Response();
        void setStatusCode(int code);
        void setHeader(const std::string& key, const std::string& value);
        void setBody(const std::string& content);
        std::string build();
};

#endif

/*
HTTP Response structure (always the same format):

[Status Line] contains the version + status code+ status message
[Headers] same as the request 
[Blank Line] \r\n
[Body] same body

*/