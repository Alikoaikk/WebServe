/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 20:17:49 by msafa             #+#    #+#             */
/*   Updated: 2026/09/29 20:17:50 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SOCKETUTILS_HPP
#define SOCKETUTILS_HPP

#include <string>

int     createSocket();
void    configureSocket(int fd);
void    bindSocket(int fd, const std::string& host, int port);
void    listenSocket(int fd);
void    setNonBlocking(int fd);

#endif
