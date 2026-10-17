/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 21:08:28 by msafa             #+#    #+#             */
/*   Updated: 2026/10/17 23:14:04 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>

struct Client;
class parse;

class Server
{
    private:
        int listenFd;
        parse::serConfig config;

        
    public:
        Server();
        Server(const parse::serConfig& cfg);
        Server(const Server& other);
        ~Server();

        int                         getListenFd() const;
        const parse::serConfig&     getConfig() const;
        void                        acceptNewClient(std::vector<Client*>& clients);
        
    private : 
        void                    	createListenSocket(const std::string& host, int port);
        Server&                 	operator=(const Server& other);
    };

#endif