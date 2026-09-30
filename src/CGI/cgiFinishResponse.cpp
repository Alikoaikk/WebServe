/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiFinishResponse.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 22:57:20 by msafa             #+#    #+#             */
/*   Updated: 2026/10/01 01:13:04 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"

static size_t checkEnd(const std::string& output, size_t& separatorLen)
{
    size_t crlf = output.find("\r\n\r\n");
    size_t lf = output.find("\n\n");

    if (crlf != std::string::npos && (lf == std::string::npos || crlf <= lf))
    {
        separatorLen = 4;
        return crlf;
    }
    if (lf != std::string::npos)
    {
        separatorLen = 2;
        return lf;
    }
    return std::string::npos;
}

static void parseCgiHeaders(Client* client, const std::string& block)
{
    size_t start = 0;

    while (start < block.length())
    {
        size_t end = block.find('\n', start);
        if (end == std::string::npos)
            end = block.length();
        std::string line = block.substr(start, end - start);
        if (!line.empty() && line[line.length() - 1] == '\r')
            line.erase(line.length() - 1);
        start = end + 1;

        size_t colon = line.find(':');
        if (colon == std::string::npos)
            continue;
        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon + 1);
        while (!value.empty() && (value[0] == ' ' || value[0] == '\t'))
            value.erase(0, 1);
        if (key == "Status")
        {
            int code = std::atoi(value.c_str());
            if (code >= 100 && code <= 599)
                client->response->setStatusCode(code);
            continue;
        }
        client->response->setHeader(key, value);
    }
}

void cgiFinishResponse(Client *client)
{
    size_t separatorLen = 0 ; 
    const std::string& output = client->cgi->output;
    size_t headerEnd = checkEnd(output, separatorLen);

    if (headerEnd == std::string::npos)
    {
        buildErrorResponse(client, 502);
        return ; 
    }
    parseCgiHeaders(client, output.substr(0, headerEnd));
    client->response->setBody(output.substr(headerEnd + separatorLen));
    finalizeResponse(client);
}
