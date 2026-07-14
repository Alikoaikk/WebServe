/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:32:41 by akoaik            #+#    #+#             */
/*   Updated: 2026/07/14 22:38:02 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/headers/imports.hpp"
#include "../../../includes/classes/serveFile_helper.hpp"
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

static std::string readFileToString(const std::string& path)
{
    std::ifstream file(path.c_str(), std::ios::binary);
	// binary : because the "\r \n" and other special character will be skiped if we read it as text.
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

static Response serveFile
(
	std::string&			fullPath,
	struct stat&			st,
	const parse::locConfig&	loc
)
static Response serveFile(std::string& fullPath, struct stat& st, const parse::locConfig& loc)
{
    if (S_ISDIR(st.st_mode))
        return handleDirectory(fullPath, loc);
    if (!S_ISREG(st.st_mode))
    {
        Response res;
        res.setStatusCode(403);
        return res;
    }
    if (access(fullPath.c_str(), R_OK) != 0)
    {
        Response res;
        res.setStatusCode(403);
        return res;
    }
    std::string body = readFileToString(fullPath);
    Response res;
    res.setStatusCode(200);
    // res.setHeader("Content-Type", getMimeType(fullPath));
    std::ostringstream len;
    len << body.size();
    res.setHeader("Content-Length", len.str());
    res.setBody(body);
    return res;
    return serveRegularFile(fullPath);
}

Response handleGet(const Request& req, const parse::serConfig& serv)
{
    const parse::locConfig* loc = NULL;

    for (size_t i = 0; i < serv.locations.size(); i++)
    {
        if (req._uri.find(serv.locations[i].path) == 0)
        {
            loc = &serv.locations[i];
            break;
        }
    }
    if (!loc)
    {
        Response res;
        res.setStatusCode(404);
        return res;
    }

    bool methodAllowed = false;
    for (size_t i = 0; i < loc->methods.size(); i++)
    {
        if (loc->methods[i] == "GET")
        {
            methodAllowed = true;
            break;
        }
    }
    if (!methodAllowed)
    {
        Response res;
        res.setStatusCode(405);
        return res;
    }

    if (loc->redirectCode != 0)
    {
        Response res;
        res.setStatusCode(loc->redirectCode);
        res.setHeader("Location", loc->redirectUrl);
        return res;
    }
	else
	{
		std::string fullPath = createPath(req._uri, *loc);
		struct stat st;
		if (stat(fullPath.c_str(), &st) == -1)
		{
			Response res;
			res.setStatusCode(404);
			return res;
		}
		return serveFile(fullPath, st, *loc);
	}

    std::string fullPath = createPath(req._uri, *loc);
    struct stat st;
    if (stat(fullPath.c_str(), &st) == -1)
    {
        Response res;
        res.setStatusCode(404);
        return res;
    }
    return serveFile(fullPath, st, *loc);
}

/*
	Most Importent Notes :

		1- stat : is a struct used from stat function
			it is a system call, that check the validation of a path in the system
			if it returned -1, so the os couldn't find the path
*/

/*
    missing till now :
        1.  Method check
        2.  no location found, should return 404
        3.  getMimeType, search about it, see where we should
            use it in the handleget function
*/
