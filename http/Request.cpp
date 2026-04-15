/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 18:32:55 by msafa             #+#    #+#             */
/*   Updated: 2026/04/15 15:07:01 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
    : _parseState(PARSE_REQUEST_LINE), _contentLength(0), _chunked(false)
{
}
Request::~Request() {}