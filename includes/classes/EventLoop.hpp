/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 22:35:23 by msafa             #+#    #+#             */
/*   Updated: 2026/05/03 22:45:27 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTLOOP_HPP
#define EVENTLOOP_HPP

#include <vector>
#include <poll.h>

struct Client;
class Server;

void checkClientTimeouts(std::vector<Client*>& connected_clients);
void buildPollArray(std::vector<struct pollfd>& fds, std::vector<Server*>& servers, std::vector<Client*>& connected_clients);
void handleClientData(std::vector<Client*>& connected_clients, std::vector<struct pollfd>& fds, size_t serverCount);
void handleClientDisconnect(std::vector<Client*>& connected_clients, size_t index);
void runEventLoop(std::vector<Server*>& servers, std::vector<Client*>& connected_clients);

#endif
