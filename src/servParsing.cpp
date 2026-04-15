/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servParsing.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:42:52 by akoaik            #+#    #+#             */
/*   Updated: 2026/04/14 16:42:53 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/classes/parsing.hpp"

static void parseListen
(
    const std::vector<std::string>& tokens,
    size_t& i,
    parse::serConfig& sc
)
{
    i++;
    std::string val = tokens[i];
    i++ ;
    size_t colon = val.rfind(':');
    if (colon != std::string::npos)
    {
        sc.host = val.substr(0, colon);
        sc.port = std::atoi(val.substr(colon + 1).c_str());
    }
    else
        sc.port = std::atoi(val.c_str());
    expectSemicolon(tokens, i);
}

static void parseClientMaxBodySize
(
    std::vector<std::string>& tokens,
    size_t& i,
    parse::serConfig& sc
)
{
    i++;
    sc.clientMaxBodySize = parseSize(tokens[i]);
    i++ ;
    expectSemicolon(tokens, i);
}

static void parseErrorPage(const std::vector<std::string>& tokens, size_t& i, parse::serConfig& sc)
{
    i++;
    int code = std::atoi(tokens[i].c_str());
    i++ ;
    std::string path = tokens[i];
    i++ ;
    sc.errorPages[code] = path;
    expectSemicolon(tokens, i);
}

static void parseLocation(std::vector<std::string>& tokens, size_t& i, parse::serConfig& sc)
{
    i++;
    parse::locConfig lc;
    lc.path = tokens[i];
    i++ ;
    lc.redirectCode = 0;
    lc.autoindex = false;
    if (i >= tokens.size() || tokens[i] != "{")
        throw std::runtime_error("expected '{' after location path");
    i++;
    parseLocationBlock(tokens, i, lc);
    sc.locations.push_back(lc);
}

void parse::parseServerBlock
(
    std::vector<std::string>& tokens,
    size_t& i,
    serConfig& sc
)
{
    while (i < tokens.size() && tokens[i] != "}")
    {
        if (tokens[i] == "listen")
            parseListen(tokens, i, sc);
        else if (tokens[i] == "client_max_body_size")
            parseClientMaxBodySize(tokens, i, sc);
        else if (tokens[i] == "error_page")
            parseErrorPage(tokens, i, sc);
        else if (tokens[i] == "location")
            parseLocation(tokens, i, sc);
        else
            throw std::runtime_error("unknown directive in server block: " + tokens[i]);
    }
    if (i >= tokens.size())
        throw std::runtime_error("expected '}' to close server block");
    if (sc.port == 0)
        throw std::runtime_error("server block missing 'listen' directive");
    i++;
}
