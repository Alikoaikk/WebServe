/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 02:32:47 by akoaik            #+#    #+#             */
/*   Updated: 2026/04/14 22:02:55 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// parsing main :

#include "includes/classes/parsing.hpp"

int main(void)
{
    try
    {
        parse p("default.conf");

        for (size_t s = 0; s < p.servers.size(); s++)
        {
            std::cout << "--- server " << s << " ---" << std::endl;
            std::cout << "host: " << p.servers[s].host << std::endl;
            std::cout << "port: " << p.servers[s].port << std::endl;
            std::cout << "client_max_body_size: " << p.servers[s].clientMaxBodySize << std::endl;

            for (size_t l = 0; l < p.servers[s].locations.size(); l++)
                std::cout << "  location: " << p.servers[s].locations[l].path << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
