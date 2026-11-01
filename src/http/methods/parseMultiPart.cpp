/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseMultiPart.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 20:14:33 by akoaik            #+#    #+#             */
/*   Updated: 2026/10/20 00:54:10 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "functions/POST_helper.hpp"

std::string parseMultipart(const std::string& body, const std::string& boundary,
                           std::string& outFilename, size_t& pos)
{
    std::string delimiter = "--" + boundary;

    while (true)
    {
        size_t start = body.find(delimiter, pos);
        if (start == std::string::npos)
            return "";
        if (body.compare(start + delimiter.size(), 2, "--") == 0)
            return "";

        size_t headerEnd = body.find("\r\n\r\n", start);
        if (headerEnd == std::string::npos)
            return "";

        size_t contentStart = headerEnd + 4;
        size_t contentEnd = body.find("\r\n" + delimiter, contentStart);
        if (contentEnd == std::string::npos)
            return "";

        std::string headers = body.substr(start, headerEnd - start);
        size_t fn = headers.find("filename=\"");
        if (fn != std::string::npos)
        {
            fn += 10;
            size_t fnEnd = headers.find('"', fn);
            if (fnEnd != std::string::npos)
                outFilename = headers.substr(fn, fnEnd - fn);
            pos = contentEnd + 2;
            return body.substr(contentStart, contentEnd - contentStart);
        }
        pos = contentEnd + 2;
    }
}
