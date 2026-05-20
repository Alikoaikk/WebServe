/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:39:00 by akoaik            #+#    #+#             */
/*   Updated: 2026/04/20 15:39:01 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHODS_HPP
#define METHODS_HPP

#include "../headers/imports.hpp"

Response handleGet(const Request& req, const parse::locConfig& loc);
Response handlePost(const Request& req, const parse::locConfig& loc);
Response handleDelete(const Request& req, const parse::locConfig& loc);

std::string createPath(const std::string& _uri, const parse::locConfig& location);

std::string getMimeType(const std::string& path);

#endif
