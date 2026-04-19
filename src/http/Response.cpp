/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 18:33:57 by msafa             #+#    #+#             */
/*   Updated: 2026/04/19 19:28:12 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/classes/Response.hpp"
#include <sstream>

static std::string getStatusMessage(int code)
{
    switch (code)
    {
    case 200:
        return "OK";
    case 400:
        return "Bad Request";
    case 404:
        return "Not Found";
    case 405:
        return "Method Not Allowed";
    case 500:
        return "Internal Server Error";
    default:
        return "Unknown";
    }
}

Response::Response()
    :_version("HTTP/1.1"), _statusCode(200),_statusMessage("OK")
{}
Response::~Response() {}

void Response::setStatusCode(int code)
{
    _statusCode = code;
    _statusMessage = getStatusMessage(code);
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