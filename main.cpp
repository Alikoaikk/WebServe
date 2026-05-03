/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 00:32:10 by akoaik            #+#    #+#             */
/*   Updated: 2026/05/03 22:45:31 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/headers/imports.hpp"

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

int main(int argc, char *argv[])
{
    try
    {
        parse config = loadConfig(argc, argv);

        std::vector<Server*> servers;
        std::vector<Client*> connected_clients;

        initializeServers(config, servers);
        runEventLoop(servers, connected_clients);
    }
    catch (const std::exception& e)
    {
        return (1);
    }

    return (0);
}
