/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 02:32:47 by akoaik            #+#    #+#             */
/*   Updated: 2026/04/14 02:33:05 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



// parsing main :

#include "includes/classes/parsing.hpp"

int main(void)
{
    std::vector<std::string> tokens = tokenize("default.conf") ;

    for (size_t i = 0; i < tokens.size(); i++)
        std::cout << "[" << i << "] " << tokens[i] << std::endl ;

    return 0 ;
}
