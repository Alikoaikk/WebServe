/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   needCGI.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 15:24:34 by akoaik            #+#    #+#             */
/*   Updated: 2026/10/19 20:38:18 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"

bool needsCgi(const std::string& uri, const parse::locConfig& loc)
{
    if (loc.cgiPass.empty())
        return (false);

    if (uri.size() < loc.cgiPass.size())
        return (false);

    std::string ext = uri.substr(uri.size() - loc.cgiPass.size());
    return (ext == loc.cgiPass);
}