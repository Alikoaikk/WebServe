/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 00:32:10 by akoaik            #+#    #+#             */
/*   Updated: 2026/10/18 19:32:05 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"
#include <csignal>

volatile sig_atomic_t g_running = 1 ;

static parse loadConfig(int argc, char *argv[])
{
    if (argc > 2)
    {
        std::cerr << "Usage: ./webserv [config_file]" << std::endl;
        throw std::runtime_error("Invalid arguments");
    }

    std::string configFile;
    if (argc != 2)
        configFile = "default.conf";
    else
        configFile = argv[1];

    try
    {
        return parse(configFile);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error parsing config: " << e.what() << std::endl;
        throw;
    }
}

static void initializeServers(const parse& config, std::vector<Server*>& servers)
{
    for (size_t i = 0; i < config.servers.size(); i++)
    {
        try
        {
            Server* server = new Server(config.servers[i]);
            servers.push_back(server);
            std::cout << "Server listening on " << config.servers[i].host << ":"
                      << config.servers[i].port << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to create server: " << e.what() << std::endl;
            throw;
        }
    }
}

static void handleSignint(int)
{
    g_running = 0 ;
}

int main(int argc, char *argv[])
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, handleSignint);

    try
    {
        parse config = loadConfig(argc, argv);

        std::vector<Server*> servers;
        std::vector<Client*> connected_clients;

        initializeServers(config, servers);
        runEventLoop(servers, connected_clients);

        for (size_t i = 0; i < connected_clients.size(); i++)
            delete connected_clients[i];
        for (size_t i = 0; i < servers.size(); i++)
            delete servers[i];
    }
    catch (const std::exception& e)
    {
        return (1);
    }
    return (0);
}
