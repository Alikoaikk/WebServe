/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 22:35:23 by msafa             #+#    #+#             */
/*   Updated: 2026/05/09 15:46:05 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTLOOP_HPP
#define EVENTLOOP_HPP

#include <vector>

struct Client;
class Server;

void runEventLoop(std::vector<Server*>& servers, std::vector<Client*>& connected_clients);

#endif
