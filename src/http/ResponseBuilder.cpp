/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 22:35:24 by msafa             #+#    #+#             */
/*   Updated: 2026/10/04 22:34:15 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"

static bool shouldKeepAlive(const Request* req)
{
    std::map<std::string, std::string>::const_iterator it = req->_headers.find("Connection");
    std::string connection;
    if(it != req->_headers.end())
        connection = it->second;
    else
        connection = "";
    if(req->_version == "HTTP/1.1")
        return connection != "close";
    return connection == "keep-alive";
}

void finalizeResponse(Client* client)
{
    if(client->response->getStatusCode() >= 400)
        client->keep_alive = false;
    else
        client->keep_alive = shouldKeepAlive(client->request);
    if(client->keep_alive)
        client->response->setHeader("Connection","keep-alive");
    else
        client->response->setHeader("Connection","close");
    client->send_buffer = client->response->build();
    client->response_ready = true;
}

static bool tryLoadErrorPage(Client* client, int code, std::string& body)
{
    std::map<int, std::string>::const_iterator it = client->serverConfig->errorPages.find(code);
    if(it != client->serverConfig->errorPages.end())
    {
        std::string path = it->second;
        if(!path.empty() && path[0] == '/')
            path = path.substr(1);
        std::ifstream file(path.c_str(), std::ios::binary);
        if(!file.is_open())
            return false;
        std::stringstream ss;
        ss << file.rdbuf();
        body = ss.str();
        return true;
    }
    else
        return false;
}

void buildErrorResponse(Client* client, int code)
{
    client->response->setStatusCode(code);
    std::string message = client->response->getStatusMessage(code);
    std::ostringstream oss;
    oss << code;
    std::string codeStr = oss.str();
    client->response->setHeader("Content-Type","text/html");
    std::string body;
    if(tryLoadErrorPage(client, code, body))
        client->response->setBody(body);
    else
        client->response->setBody("<html><body><h1>" + codeStr + " " + message + "</h1></body></html>");
    finalizeResponse(client);
}

static bool isMethodAllowed(const parse::locConfig* loc, const std::string& method)
{
    for(size_t j = 0; j < loc->methods.size(); j++)
        if(loc->methods[j] == method)
            return true;
    return false;
}

static bool bodyTooLarge(const Request* req, size_t limit)
{
    if(req->_contentLength > limit)
        return true;
    if(req->_body.length() > limit)
        return true;
    return false;
}

static bool dispatchRequest(Client* client, const parse::locConfig* loc)
{
    const Request& req = *client->request;
    const parse::serConfig& serv = *client->serverConfig;
    if(req._method != "GET" && req._method != "POST" && req._method != "DELETE")
    {
        buildErrorResponse(client, 501);
        return true;
    }
    std::string fullPath = createPath(req._uri, *loc);
    methods m;

    if (needsCgi(fullPath, *loc))
    {
        client->cgi = cgiBuildResponse(req, *loc, fullPath);
        if (client->cgi == NULL)
            buildErrorResponse(client, 500);
        return true; 
    }
    else if (req._method == "GET")
        *client->response = m.handleGet(req, serv);
    else if (req._method == "DELETE")
        *client->response = m.handleDelete(req, serv);
    else if (req._method == "POST")
        *client->response = m.handlePost(req, serv);

    return false ;
}

static void buildResponse(Client* client)
{
    const parse::locConfig* loc = findLocation(client->request->_uri, *client->serverConfig);
    if (loc == NULL)
    {
        buildErrorResponse(client, 404);
        return;
    }
    if (!isMethodAllowed(loc, client->request->_method))
    {
        buildErrorResponse(client, 405);
        return;
    }
    if (loc->root.empty() && loc->redirectCode == 0)
    {
        buildErrorResponse(client, 500);
        return;
    }
    if (dispatchRequest(client, loc))
        return ;
    
    int status = client->response->getStatusCode();
    if (status >= 400 && client->response->getBody().empty())
        buildErrorResponse(client, status);
    else
        finalizeResponse(client);
}

void processClientRequest(Client* client)
{
    if (client->cgi != NULL)
        return ;
    size_t limit = client->serverConfig->clientMaxBodySize;
    if (bodyTooLarge(client->request, limit))
        buildErrorResponse(client, 413);
    else if (client->request->_parseState == PARSE_COMPLETE)
        buildResponse(client);
    else if (client->request->_parseState == PARSE_ERROR)
        buildErrorResponse(client, 400);
}
