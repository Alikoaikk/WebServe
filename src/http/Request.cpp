/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 18:32:55 by msafa             #+#    #+#             */
/*   Updated: 2026/04/19 19:59:40 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/classes/Request.hpp"
#include <sstream>
#include <cstdlib>

Request::Request()
    : _parseState(PARSE_REQUEST_LINE), _contentLength(0), _chunked(false)
{}
Request::~Request() {}

static void parseRequestLine(std::string& rawBuffer, ParseState& parseState, std::string& method, std::string& uri, std::string& version)
{
    size_t pos = rawBuffer.find("\r\n");
    if(pos != std::string::npos)
    {
        std::istringstream iss(rawBuffer.substr(0, pos));
        iss >> method;
        iss >> uri;
        iss >> version;
        if(version != "HTTP/1.0" && version != "HTTP/1.1")
        {
            parseState = PARSE_ERROR;
            return;
        }
        if(method != "GET" && method != "POST" && method != "DELETE")
        {
            parseState = PARSE_ERROR;
            return;
        }
        rawBuffer = rawBuffer.substr(pos + 2);
        parseState = PARSE_HEADERS;
    }
}

static void parseHeaderLine(const std::string& headerLine, std::map<std::string, std::string>& headers, size_t& contentLength, bool& chunked)
{
    size_t colonPos = headerLine.find(":");
    if(colonPos != std::string::npos)
    {
        std::string key = headerLine.substr(0, colonPos);
        std::string value = headerLine.substr(colonPos + 1);
        std::istringstream iss(value);
        iss >> value;
        headers[key] = value;
        if(key == "Content-Length")
        {
            std::istringstream iss(value);
            iss >> contentLength;
        }
        if(key == "Transfer-Encoding" && value == "chunked")
            chunked = true;
    }
}

static void parseHeaders(std::string& rawBuffer, ParseState& parseState, std::map<std::string, std::string>& headers, size_t& contentLength, bool& chunked)
{
    while(true)
    {
        size_t pos = rawBuffer.find("\r\n");
        if(pos == std::string::npos)
            return;
        if(pos == 0)
        {
            rawBuffer = rawBuffer.substr(2);
            parseState = PARSE_BODY;
            return;
        }
        std::string headerLine = rawBuffer.substr(0, pos);
        parseHeaderLine(headerLine, headers, contentLength, chunked);
        rawBuffer = rawBuffer.substr(pos + 2);
    }
}

static void parseBodyContentLength(std::string& rawBuffer, ParseState& parseState, std::string& body, size_t contentLength)
{
    if(rawBuffer.length() >= contentLength)
    {
        body = rawBuffer.substr(0, contentLength);
        rawBuffer = rawBuffer.substr(contentLength);
        parseState = PARSE_COMPLETE;
    }
}

static void parseBodyChunked(std::string& rawBuffer, ParseState& parseState, std::string& body)
{
    while(true)
    {
        size_t pos = rawBuffer.find("\r\n");
        if(pos != std::string::npos)
        {
            std::string hex = rawBuffer.substr(0, pos);
            char *end = NULL;
            unsigned long chunkSize = std::strtoul(hex.c_str(), &end, 16);
            if(end != hex.c_str() + hex.length() || end == hex.c_str())
            {
                parseState = PARSE_ERROR;
                return;
            }
            if(chunkSize == 0)
            {
                parseState = PARSE_COMPLETE;
                break;
            }
            if(chunkSize > 0)
            {
                size_t totalNeeded = pos + 2 + chunkSize + 2;
                if(rawBuffer.size() < totalNeeded)
                    return;
                body.append(rawBuffer.substr(pos + 2, chunkSize));
                rawBuffer = rawBuffer.substr(totalNeeded);
                continue;
            }
        }
        else
            return;
    }
}

static void parseBody(std::string& rawBuffer, ParseState& parseState, std::string& body, size_t contentLength, bool chunked)
{
    if(!chunked)
        parseBodyContentLength(rawBuffer, parseState, body, contentLength);
    else
        parseBodyChunked(rawBuffer, parseState, body);
}

void Request::parse(std::string& chunk)
{
    _rawBuffer += chunk;
    if(_parseState == PARSE_REQUEST_LINE)
        parseRequestLine(_rawBuffer, _parseState, _method, _uri, _version);
    if(_parseState == PARSE_HEADERS)
        parseHeaders(_rawBuffer, _parseState, _headers, _contentLength, _chunked);
    if(_parseState == PARSE_BODY)
        parseBody(_rawBuffer, _parseState, _body, _contentLength, _chunked);
}
