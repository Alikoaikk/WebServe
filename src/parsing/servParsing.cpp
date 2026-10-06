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

#include "classes/imports.hpp"

static void parseListen(const std::vector<std::string> &tokens, size_t &i,
                        parse::serConfig &sc)
{
    i++;
    if (i >= tokens.size())
        throw std::runtime_error("unexpected end of file after 'listen'");
    std::string val = tokens[i];
    i++;
    size_t colon = val.rfind(':');
    std::string portStr;
    if (colon != std::string::npos)
    {
        sc.host = val.substr(0, colon);
        portStr = val.substr(colon + 1);
    }
    else
        portStr = val;
    int port = std::atoi(portStr.c_str());
    if (port < 1 || port > 65535)
        throw std::runtime_error("invalid port: " + portStr);
    sc.port = port;
    expectSemicolon(tokens, i);
}

static void parseClientMaxBodySize(std::vector<std::string> &tokens, size_t &i,
                                   parse::serConfig &sc)
{
    i++;
    if (i >= tokens.size())
        throw std::runtime_error("unexpected end of file after 'client_max_body_size'");
    sc.clientMaxBodySize = parseSize(tokens[i]);
    i++;
    expectSemicolon(tokens, i);
}

static void parseErrorPage(const std::vector<std::string> &tokens, size_t &i,
                           parse::serConfig &sc) {
  i++;
  if (i >= tokens.size())
    throw std::runtime_error("unexpected end of file after 'error_page'");
  int code = std::atoi(tokens[i].c_str());
  i++;
  if (i >= tokens.size())
    throw std::runtime_error("unexpected end of file: expected path after error code");
  std::string path = tokens[i];
  i++;
  sc.errorPages[code] = path;
  expectSemicolon(tokens, i);
}

static void parseLocation(std::vector<std::string> &tokens, size_t &i,
                          parse::serConfig &sc)
{
    i++;
    if (i >= tokens.size())
        throw std::runtime_error("unexpected end of file after 'location'");
    parse::locConfig lc;
    lc.path = tokens[i];
    i++;
    lc.redirectCode = 0;
    lc.autoindex = false;
    if (i >= tokens.size() || tokens[i] != "{")
        throw std::runtime_error("expected '{' after location path");
    i++;
    parseLocationBlock(tokens, i, lc);
    sc.locations.push_back(lc);
}

void parse::parseServerBlock(std::vector<std::string> &tokens, size_t &i,
                             serConfig &sc)
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
