/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 18:33:57 by msafa             #+#    #+#             */
/*   Updated: 2026/04/18 22:10:20 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/classes/Response.hpp"
#include <sstream>

Response::Response()
    :_version("HTTP/1.1"), _statusCode(200),_statusMessage("OK")
{}
Response::~Response() {}

void Response::setStatusCode(int code)
{
    _statusCode = code;
}

void Response::setHeader(const std::string& key, const std::string& value)
{
    _header[key] = value;
}

void Response::setBody(const std::string& content)
{
    _body = content;
}

std::string Response::build()
{
    std::string result;

    std::ostringstream oss;
    oss << _statusCode;
    std::string statusCode = oss.str();
    
    result += _version 
                + " " 
                + statusCode
                + " "
                + _statusMessage
                +"\r\n";
    std::map<std::string, std::string>::iterator itr;
    
    for(itr = _header.begin(); itr != _header.end() ; ++itr)
    {
        result += itr->first + ": " + itr->second + "\r\n";
    }
    result += "\r\n";
    result += _body;
    return result;
}