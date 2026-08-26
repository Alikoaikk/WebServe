/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 11:08:16 by akoaik            #+#    #+#             */
/*   Updated: 2026/08/26 10:57:07 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"
#include "classes/serveFile_helper.hpp"


static std::string checkBasePath(const parse::locConfig& location)
{
    if (!location.uploadStore.empty())
        return location.uploadStore;
    if (!location.root.empty())
        return location.root;
    return "";
}

static std::string buildPath(const std::string& uri, const parse::locConfig& location, std::string basePath)
{
    std::string remainder = uri;
    if (uri.find(location.path) == 0)
        remainder = uri.substr(location.path.size());
    if (!basePath.empty() && basePath[basePath.size() - 1] == '/')
        basePath = basePath.substr(0, basePath.size() - 1);
    if (remainder.empty() || remainder == "/")
        return "";
    if (remainder[0] != '/')
        remainder = "/" + remainder;
    return basePath + remainder;
}

static Response uploadFile(const std::string& fullPath, const std::string& body, const std::string& uri)
{
    std::ofstream file(fullPath.c_str(), std::ios::binary | std::ios::trunc);
    if (!file.is_open())
    {
        Response res;
        res.setStatusCode(403);
        return res;
    }
    file.write(body.data(), body.size());
    file.close();

    Response res;
    res.setStatusCode(201);
    res.setHeader("Content-Type", "text/plain");
    res.setHeader("Location", uri);
    res.setBody("Created");
    return res;
}

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

    std::string fullPath = buildPath(req._uri, *location, basePath);
    if (fullPath.empty())
    {
        Response res;
        res.setStatusCode(400);
        return res;
    }

    return uploadFile(fullPath, req._body, req._uri);
}