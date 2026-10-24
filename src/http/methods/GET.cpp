/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:32:41 by akoaik            #+#    #+#             */
/*   Updated: 2026/10/19 23:20:33 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"
#include "classes/serveFile_helper.hpp"
#include <cstddef>
#include <sys/stat.h>

std::string createPath(const std::string& _uri, const parse::locConfig& location)
{
    std::string remainder = _uri;
    if (_uri.find(location.path) == 0)
        remainder = _uri.substr(location.path.size());
    std::string root = location.root;
    if (!root.empty() && root[root.size() - 1] == '/')
        root = root.substr(0, root.size() - 1);
    if (remainder.empty() || remainder[0] != '/')
        remainder = "/" + remainder;
    return root + remainder;
}

static Response serveFile
(
	std::string&			fullPath,
	struct stat&			st,
	const parse::locConfig&	loc
)
{
    if (S_ISDIR(st.st_mode))
        return handleDirectory(fullPath, loc);
    if (!S_ISREG(st.st_mode))
    {
        Response res;
        res.setStatusCode(403);
        return res;
    }
    return serveRegularFile(fullPath);
}

const parse::locConfig* findLocation(const std::string& uri, const parse::serConfig& serv)
{
    const parse::locConfig* loc = NULL;
    size_t bestLen = 0;
    size_t size;

    for (size_t i = 0; i < serv.locations.size(); i++)
    {
        size = serv.locations[i].path.size();
        if (uri.find(serv.locations[i].path) == 0 &&
            (uri.size() == size || uri[size] == '/' || serv.locations[i].path[size - 1] == '/') &&
            size > bestLen)
        {
            loc = &serv.locations[i];
            bestLen = size;
        }
    }
    return loc;
}

Response methods::handleGet(const Request& req, const parse::serConfig& serv)
{
    const parse::locConfig* loc = findLocation(req._uri, serv);
    if (!loc)
    {
        Response res;
        res.setStatusCode(404);
        return res;
    }

    std::string fullPath = createPath(req._uri, *loc);
    struct stat st;
    if (stat(fullPath.c_str(), &st) == -1)
    {
        Response res;
        res.setStatusCode(404);
        return res;
    }
    if (S_ISDIR(st.st_mode) && req._uri[req._uri.size() - 1] != '/')
    {
        Response res;
        res.setStatusCode(301);
        res.setHeader("Location", req._uri + "/");
        return res;
    }
    return serveFile(fullPath, st, *loc);
}
