/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 21:08:28 by msafa             #+#    #+#             */
/*   Updated: 2026/04/18 23:34:46 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>

class Socket
{
    public:
        static int createListenSocket(const std::string& host, int port);
        static void setNonBlocking(int fd);
};

#endif