/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 02:32:47 by akoaik            #+#    #+#             */
/*   Updated: 2026/04/15 16:45:06 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// parsing main :

#include "includes/classes/parsing.hpp"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./webserv <config file>" << std::endl;
        return 1;
    }
    try
    {
        parse p(argv[1]);
        printConfig(p);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
