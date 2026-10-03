/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 18:33:57 by msafa             #+#    #+#             */
/*   Updated: 2026/10/03 17:06:25 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"

std::string Response::getStatusMessage(int code)
{
    switch (code)
    {
    case 200: return "OK";
    case 201: return "Created";
    case 204: return "No Content";
    case 301: return "Moved Permanently";
    case 302: return "Found";
    case 303: return "See Other";
    case 307: return "Temporary Redirect";
    case 308: return "Permanent Redirect";
    case 400: return "Bad Request";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 405: return "Method Not Allowed";
    case 408: return "Request Timeout";
    case 413: return "Payload Too Large";
    case 500: return "Internal Server Error";
    case 501: return "Not Implemented";
    case 502: return "Bad Gateway";
    case 504: return "Gateway Timeout";
    case 505: return "HTTP Version Not Supported";
    default: return "Unknown";  
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

int Response::getStatusCode() const
{
    return _statusCode;
}

const std::string& Response::getBody() const
{
    return _body;
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
        if(itr->first == "Content-Length")
            continue;
        result += itr->first + ": " + itr->second + "\r\n";
    }
    std::ostringstream lenStream;
    lenStream << _body.length();
    result += "Content-Length: " + lenStream.str() + "\r\n";
    result += "\r\n";
    result += _body;
    return result;
}
