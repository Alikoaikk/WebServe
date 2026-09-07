/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiBuildResponse.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 21:18:25 by akoaik            #+#    #+#             */
/*   Updated: 2026/09/07 15:52:48 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"
#include <sys/wait.h>

static void buildEnv
(
    std::vector<std::string>&	env,
    const Request&				req,
    const std::string&			fullPath
)
{
    std::ostringstream oss;
    oss << req._body.size();

    env.push_back("REQUEST_METHOD=" + req._method);
    env.push_back("QUERY_STRING=" + req._queryString);
    env.push_back("CONTENT_LENGTH=" + oss.str());
    env.push_back("SCRIPT_FILENAME=" + fullPath);
    env.push_back("PATH_INFO=");
    env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    env.push_back("SERVER_PROTOCOL=HTTP/1.1");
}

Response cgiBuildResponse(const Request& req, const parse::locConfig& loc, const std::string& fullPath)
{
    Response res;

    std::vector<std::string> env;
    buildEnv(env, req, fullPath);

    return res;
}
