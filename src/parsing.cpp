/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 02:01:45 by akoaik            #+#    #+#             */
/*   Updated: 2026/04/14 02:40:31 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/classes/parsing.hpp"

std::vector<std::string> tokenize(const std::string &filename)
{
    std::vector<std::string>    tokens ;
    std::ifstream               file(filename.c_str()) ;
    std::string                 line ;

    if (!file.is_open())
        throw std::runtime_error("cannot open config file: " + filename) ;

    while (std::getline(file, line))
    {
        std::istringstream iss(line) ;
        std::string        word ;
        while (iss >> word) // split the line
        {
            if (word[0] == '#') // for comments
                break ;
            std::string token ;
            for (size_t i = 0; i < word.size(); i++)
            {
                char c = word[i] ;
                if (c == '{' || c == '}' || c == ';')
                {
                    if (!token.empty())
                    {
                        tokens.push_back(token) ;
                        token.clear() ;
                    }
                    tokens.push_back(std::string(1, c)) ;
                }
                else
                    token += c ;
            }
            if (!token.empty())
                tokens.push_back(token) ;
        }
    }
    return tokens ;
}

parse::parse(std::string configFileName)
    : configFileName(configFileName)
{
    std::vector<std::string> tokens ;

    tokens = tokenize(configFileName) ;
}

