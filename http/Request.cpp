/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 18:32:55 by msafa             #+#    #+#             */
/*   Updated: 2026/04/17 23:00:32 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <sstream>

Request::Request()
    : _parseState(PARSE_REQUEST_LINE), _contentLength(0), _chunked(false)
{}
Request::~Request() {}

void Request::parse(std::string& chunk)
{
    _rawBuffer += chunk;
    if(_parseState == PARSE_REQUEST_LINE)
    {
        size_t pos = _rawBuffer.find("\r\n");
        if(pos != std::string::npos)
        {
            std::istringstream iss(_rawBuffer.substr(0,pos));
            iss >> _method;
            iss >> _uri;
            iss >> _version;
            _rawBuffer = _rawBuffer.substr(pos + 2);
            _parseState = PARSE_HEADERS;
        }
        else
            return;
    }
    if(_parseState == PARSE_HEADERS)
    {
        size_t pos = _rawBuffer.find("\r\n");
        if(pos == std::string::npos)
            return;
        if(pos == 0)
        {
            _rawBuffer = _rawBuffer.substr(2);
            _parseState = PARSE_BODY;
            return;
        }
        std::string headerLine = _rawBuffer.substr(0,pos);
        size_t colonPos = headerLine.find(":");
        if(colonPos != std::string::npos)
        {
            std::string key,value;
            key = headerLine.substr(0,colonPos);
            value = headerLine.substr(colonPos + 1);
            std::istringstream iss(value);
            iss >> value;
            _headers[key] = value;
            if(key == "Content-Lenght")
            {
                std::istringstream iss(value);
                iss >> _contentLength;
            }
            if(key == "Transfer-Encoding")
                _chunked = true;
            _rawBuffer = _rawBuffer.substr(pos + 2);
        }
        else
            return;
    }
    if(_parseState == PARSE_BODY)
    {
        if(_rawBuffer.length() >= _contentLength)
        {
            
            _body = _rawBuffer.substr(0,_contentLength);
            _rawBuffer = _rawBuffer.substr(_contentLength);
            _parseState = PARSE_COMPLETE;
        }
        else
            return;
    }
}