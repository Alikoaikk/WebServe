/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_helper.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 01:56:53 by msafa             #+#    #+#             */
/*   Updated: 2026/10/03 01:57:01 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HELPER_HPP
#define REQUEST_HELPER_HPP

#include <string>

bool urlDecode(const std::string& in, std::string& out);
bool normalizeUri(const std::string& in, std::string& out);

#endif
