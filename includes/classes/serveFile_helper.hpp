/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveFile_helper.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 21:49:31 by akoaik            #+#    #+#             */
/*   Updated: 2026/07/13 21:49:36 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVEFILE_HELPER_HPP
#define SERVEFILE_HELPER_HPP

#include "Response.hpp"
#include "Parsing.hpp"
#include <string>

Response buildAutoindex(const std::string& fullPath);
Response serveRegularFile(const std::string& fullPath);
Response handleDirectory(std::string& fullPath, const parse::locConfig& loc);

#endif
