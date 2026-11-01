/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientIO.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 22:35:23 by msafa             #+#    #+#             */
/*   Updated: 2026/10/01 22:39:49 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTIO_HPP
#define CLIENTIO_HPP

#include <vector>
#include <poll.h>

struct Client;

void handleClientData(std::vector<Client*>& connected_clients, std::vector<struct pollfd>& fds);
void handleClientSend(std::vector<Client*>& connected_clients, std::vector<struct pollfd>& fds);
void handleClientDisconnect(std::vector<Client*>& connected_clients, size_t index);
short getRevents(const std::vector<struct pollfd>& fds, int fd);

#endif
