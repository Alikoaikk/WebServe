/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 02:01:45 by akoaik            #+#    #+#             */
/*   Updated: 2026/04/14 22:06:59 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/headers/imports.hpp"

parse::parse(std::string configFileName)
    : configFileName(configFileName)
{
    std::vector<std::string> tokens ;

    tokens = tokenize(configFileName) ;
    parseTokens(tokens) ;
}

void parse::parseTokens(std::vector<std::string> tokens)
{
    size_t i = 0;

    if (i >= tokens.size() || tokens[i] != "http")
        throw std::runtime_error("expected 'http' at start of config");
    i++;
    if (i >= tokens.size() || tokens[i] != "{")
        throw std::runtime_error("expected '{' after 'http'");
    i++;
    while (i < tokens.size() && tokens[i] != "}")
    {
        if (tokens[i] != "server")
            throw std::runtime_error("unexpected token inside http block: " + tokens[i]);
        i++;
        if (i >= tokens.size() || tokens[i] != "{")
            throw std::runtime_error("expected '{' after 'server'");
        i++;
        serConfig sc;
        parseServerBlock(tokens, i, sc);
        this->servers.push_back(sc);
    }
    if (i >= tokens.size())
        throw std::runtime_error("expected '}' to close 'http' block");
    i++;
}


