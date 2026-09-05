/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   needCGI.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 15:24:34 by akoaik            #+#    #+#             */
/*   Updated: 2026/09/05 21:19:21 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"

bool needsCgi(const std::string& fullPath, const parse::locConfig& loc)
{
    if (loc.cgiPass.empty())
        return false;

    std::string path = fullPath;
    size_t c = path.find('?');
    if (c != std::string::npos)
        path = path.substr(0, c);

    if (path.size() < loc.cgiPass.size())
        return false;
    std::string ext = path.substr(path.size() - loc.cgiPass.size());
    return ext == loc.cgiPass;
}
