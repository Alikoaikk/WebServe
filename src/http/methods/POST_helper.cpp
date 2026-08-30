/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST_helper.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 16:17:25 by akoaik            #+#    #+#             */
/*   Updated: 2026/08/30 20:41:37 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"
#include "classes/POST_helper.hpp"

std::string checkBasePath(const parse::locConfig& location)
{
    if (!location.uploadStore.empty())
        return location.uploadStore;
    if (!location.root.empty())
        return location.root;
    return "";
}

std::string buildPath
(
    const std::string&			uri,
    const parse::locConfig&		location,
    std::string					basePath
)
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

std::string getBoundary(const std::string& contentType)
{
    if (contentType.find("multipart/form-data") != 0)
        return "";
    size_t pos = contentType.find("boundary=");
    if (pos == std::string::npos)
        return "";
    std::string boundary = contentType.substr(pos + 9);
    if (boundary.size() >= 2 && boundary[0] == '"' && boundary[boundary.size() - 1] == '"')
        boundary = boundary.substr(1, boundary.size() - 2);
    return boundary;
}

Response uploadFile(const std::string& fullPath, const std::string& body, const std::string& uri)
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
