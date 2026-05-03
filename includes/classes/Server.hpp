/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 21:08:28 by msafa             #+#    #+#             */
/*   Updated: 2026/05/03 22:29:09 by msafa            ###   ########.fr       */
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
        void createListenSocket(const std::string& host, int port);
        void setNonBlocking(int fd);
    public:
    //OCF
        Server();
        Server(const parse::serConfig& cfg);
        Server(const Server& other);
        Server& operator=(const Server& other);
        ~Server();
    //methods
        int getListenFd() const;
        const parse::serConfig& getConfig() const;
        void acceptNewClient(std::vector<Client*>& clients);
};

#endif