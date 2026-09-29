/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientIO.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 22:35:23 by msafa             #+#    #+#             */
/*   Updated: 2026/05/09 15:46:05 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTIO_HPP
#define CLIENTIO_HPP

#include <vector>
#include <poll.h>

struct Client;

void handleClientData(std::vector<Client*>& connected_clients, std::vector<struct pollfd>& fds, size_t serverCount);
void handleClientSend(std::vector<Client*>& connected_clients, std::vector<struct pollfd>& fds, size_t serverCount);
void handleClientDisconnect(std::vector<Client*>& connected_clients, size_t index);

#endif
