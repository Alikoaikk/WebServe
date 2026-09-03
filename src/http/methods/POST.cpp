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

#include "classes/imports.hpp"
#include "classes/serveFile_helper.hpp"
#include "classes/POST_helper.hpp"

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
    std::map<std::string, std::string>::const_iterator it = req._headers.find("Content-Type");
    if (it != req._headers.end())
        boundary = getBoundary(it->second);

    if (!boundary.empty())
    {
        std::string filename;
        body = parseMultipart(req._body, boundary, filename);
        if (body.empty() || filename.empty())
        {
            Response res;
            res.setStatusCode(400);
            return res;
        }
        fullPath = basePath + "/" + filename;
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
