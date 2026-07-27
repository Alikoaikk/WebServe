/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveFile_helper.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 21:49:51 by akoaik            #+#    #+#             */
/*   Updated: 2026/07/20 20:13:51 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"
#include "classes/serveFile_helper.hpp"
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>
#include <sstream>

Response buildAutoindex(const std::string& fullPath)
{
    std::string body = "<html><body><h1>Index of " + fullPath + "</h1><hr><pre>";
    DIR* dir = opendir(fullPath.c_str());
    if (dir)
    {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL)
        {
            std::string name = entry->d_name;
            if (name == ".")
                continue;
            body += "<a href=\"" + name + "\">" + name + "</a>\n";
        }
        closedir(dir);
    }
    body += "</pre><hr></body></html>";

    Response res;
    res.setStatusCode(200);
    res.setHeader("Content-Type", "text/html");
    std::ostringstream len;
    len << body.size();
    res.setHeader("Content-Length", len.str());
    res.setBody(body);
    return res;
}

Response serveRegularFile(const std::string& fullPath)
{
    if (access(fullPath.c_str(), R_OK) != 0)
    {
        Response res;
        res.setStatusCode(403);
        return res;
    }
    std::ifstream file(fullPath.c_str(), std::ios::binary);
    std::stringstream ss;
    ss << file.rdbuf();
    std::string body = ss.str();

    Response res;
    res.setStatusCode(200);
    res.setHeader("Content-Type", getMimeType(fullPath));
    std::ostringstream len;
    len << body.size();
    res.setHeader("Content-Length", len.str());
    res.setBody(body);
    return res;
}

Response handleDirectory(std::string& fullPath, const parse::locConfig& loc)
{
    if (!loc.index.empty())
    {
        std::string filePath = fullPath + "/" + loc.index;
        struct stat st;
        if (stat(filePath.c_str(), &st) == 0 && S_ISREG(st.st_mode))
        {
            fullPath = filePath;
            return serveRegularFile(fullPath);
        }
        if (loc.autoindex)
            return buildAutoindex(fullPath);
        Response res;
        res.setStatusCode(403);
        return res;
    }
    if (loc.autoindex)
        return buildAutoindex(fullPath);
    Response res;
    res.setStatusCode(403);
    return res;
}
