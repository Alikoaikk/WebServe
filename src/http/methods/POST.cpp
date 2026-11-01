/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 11:08:16 by akoaik            #+#    #+#             */
/*   Updated: 2026/09/03 21:57:02 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "imports.hpp"
#include "functions/serveFile_helper.hpp"
#include "functions/POST_helper.hpp"

Response methods::handlePost(const Request& req, const parse::serConfig& serv)
{
    const parse::locConfig *location =  findLocation(req._uri, serv);
    if (!location)
    {
        Response res ;
        res.setStatusCode(404);
        return res ;
    }

    std::string basePath = checkBasePath(*location);
    if (basePath.empty())
    {
        Response res;
        res.setStatusCode(500);
        return res;
    }

    std::string body = req._body;
    std::string fullPath;

    std::string boundary;
    std::map<std::string, std::string>::const_iterator it = req._headers.find("content-type");
    if (it != req._headers.end())
        boundary = getBoundary(it->second);

    if (!boundary.empty())
    {
        size_t pos = 0;
        Response res;
        res.setStatusCode(400);
        while (true)
        {
            std::string filename;
            body = parseMultipart(req._body, boundary, filename, pos);
            if (filename.empty())
                return res;
            if (filename.find('/') != std::string::npos || filename == "." || filename == "..")
            {
                Response bad;
                bad.setStatusCode(400);
                return bad;
            }
            res = uploadFile(basePath + "/" + filename, body, req._uri);
            if (res.getStatusCode() != 201)
                return res;
        }
    }
    else
    {
        fullPath = buildPath(req._uri, *location, basePath);
        if (fullPath.empty())
        {
            Response res;
            res.setStatusCode(400);
            return res;
        }
    }
    return uploadFile(fullPath, body, req._uri);
}
