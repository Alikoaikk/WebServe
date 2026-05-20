/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:32:41 by akoaik            #+#    #+#             */
/*   Updated: 2026/04/20 18:48:58 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/classes/methods.hpp"
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

/*
    Build a small HTML error response (404, 403, 501, ...).
*/
static Response errorResponse(int code, const std::string& message)
{
    Response res;
    res.setStatusCode(code);

    std::ostringstream bodyStream;
    bodyStream << "<html><body><h1>" << code << " " << message
               << "</h1></body></html>";
    std::string body = bodyStream.str();

    std::ostringstream lenStream;
    lenStream << body.size();

    res.setHeader("Content-Type", "text/html");
    res.setHeader("Content-Length", lenStream.str());
    res.setBody(body);
    return res;
}

/*
    Read a file's full content into a std::string (binary-safe).
*/
static std::string readFileToString(const std::string& path)
{
    std::ifstream file(path.c_str(), std::ios::binary);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

Response handleGet(const Request& req, const parse::locConfig& loc)
{
    /* 1. Map the URI to a filesystem path. */
    std::string path = createPath(req._uri, loc);

    /* 2. stat() the path. If it doesn't exist -> 404. */
    struct stat st;
    if (stat(path.c_str(), &st) != 0)
        return errorResponse(404, "Not Found");

    /* 3. If it's a directory: try the index file, else autoindex, else 403. */
    if (S_ISDIR(st.st_mode))
    {
        if (!loc.index.empty())
        {
            std::string indexPath = path;
            if (!indexPath.empty() && indexPath[indexPath.size() - 1] != '/')
                indexPath += "/";
            indexPath += loc.index;

            if (stat(indexPath.c_str(), &st) == 0 && S_ISREG(st.st_mode))
                path = indexPath;
            else if (loc.autoindex)
                return errorResponse(501, "Not Implemented"); /* TODO: autoindex */
            else
                return errorResponse(403, "Forbidden");
        }
        else if (loc.autoindex)
            return errorResponse(501, "Not Implemented"); /* TODO: autoindex */
        else
            return errorResponse(403, "Forbidden");
    }
    /* 3b. Not directory and not regular file (device, fifo, ...) -> refuse. */
    else if (!S_ISREG(st.st_mode))
        return errorResponse(403, "Forbidden");

    /* 4. Make sure we can actually read it. */
    if (access(path.c_str(), R_OK) != 0)
        return errorResponse(403, "Forbidden");

    /* 5. Read the file. */
    std::string body = readFileToString(path);

    /* 6. Build the 200 OK response. */
    Response res;
    res.setStatusCode(200);
    res.setHeader("Content-Type", getMimeType(path));

    std::ostringstream lenStream;
    lenStream << body.size();
    res.setHeader("Content-Length", lenStream.str());

    res.setBody(body);
    return res;
}
