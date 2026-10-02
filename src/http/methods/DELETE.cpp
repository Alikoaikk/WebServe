/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 20:13:30 by akoaik            #+#    #+#             */
/*   Updated: 2026/10/02 23:57:44 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"
#include "classes/serveFile_helper.hpp"
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>

Response methods::handleDelete(const Request& req, const parse::serConfig& ser)
{
    const parse::locConfig* loc = findLocation(req._uri, ser);
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
    if (S_ISDIR(st.st_mode))
    {
        Response res;
        res.setStatusCode(403);
        return res;
    }
    if (std::remove(fullPath.c_str()) != 0)
    {
        Response res;
        res.setStatusCode(403);
        return res;
    }
    Response res;
    res.setStatusCode(204);
    return res;
}
