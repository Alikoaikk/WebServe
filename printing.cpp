#include "includes/classes/parsing.hpp"
#include <iostream>

void printConfig(const parse& p)
{
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
