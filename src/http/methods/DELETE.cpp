/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 20:13:30 by akoaik            #+#    #+#             */
/*   Updated: 2026/07/23 19:32:26 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/headers/imports.hpp"
#include "../../../includes/classes/serveFile_helper.hpp"
#include <sys/stat.h>
#include <unistd.h>

Response handleDelete(const Request& req, const parse::serConfig& ser)
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
    if (unlink(fullPath.c_str()) == -1)
    {
        Response res;
        res.setStatusCode(403);
        return res;
    }
    Response res;
    res.setStatusCode(204);
    return res;
}
