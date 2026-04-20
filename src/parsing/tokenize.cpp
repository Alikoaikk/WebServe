/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 15:11:09 by akoaik            #+#    #+#             */
/*   Updated: 2026/04/14 17:24:22 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

<<<<<<< HEAD
#include "../../includes/classes/parsing.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>
=======
#include "../../includes/headers/imports.hpp"
>>>>>>> main

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

void expectSemicolon(const std::vector<std::string>& tokens, size_t& i)
{
    if (i >= tokens.size())
        throw std::runtime_error("expected ';' but reached end of file");
    if (tokens[i] != ";")
        throw std::runtime_error("expected ';' near token " + tokens[i]);
    i++;
}

unsigned int parseSize(const std::string& str)
{
    if (str.empty())
        throw std::runtime_error("invalid size value");

    char            end ;
    unsigned int    byteVal ;

    end = str [str.size() - 1];
    byteVal = (unsigned int)std::atoi(str.c_str());

    if (end == 'M' || end == 'm')
        return byteVal * 1024 * 1024;
    if (end == 'K' || end == 'k')
        return byteVal * 1024;
    return byteVal;
}
