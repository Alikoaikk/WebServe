/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST_helper.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 16:06:46 by akoaik            #+#    #+#             */
/*   Updated: 2026/08/30 16:20:05 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_HELPER_HPP
#define POST_HELPER_HPP

#include "Response.hpp"
#include "Parsing.hpp"
#include <string>

std::string checkBasePath(const parse::locConfig& location);
std::string buildPath(const std::string& uri, const parse::locConfig& location, std::string basePath);
std::string getBoundary(const std::string& contentType);
std::string parseMultipart(const std::string& body, const std::string& boundary, std::string& outFilename);
Response uploadFile(const std::string& fullPath, const std::string& body, const std::string& uri);

#endif
