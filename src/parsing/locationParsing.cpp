/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationParsing.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:18:53 by akoaik            #+#    #+#             */
/*   Updated: 2026/04/15 16:31:24 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

<<<<<<< HEAD
#include "../../includes/classes/parsing.hpp"
#include <cstdlib>
=======
#include "../../includes/headers/imports.hpp"
>>>>>>> main

static void parseMethods
(
	std::vector<std::string>&	tokens,
    size_t&						i,
    parse::locConfig&			lc
)
{
    i++;
    while (i < tokens.size() && tokens[i] != ";")
    {
        lc.methods.push_back(tokens[i]);
        i++;
    }
    expectSemicolon(tokens, i);
}

static void parseRoot
(
    std::vector<std::string>&	tokens,
    size_t&						i,
    parse::locConfig&			lc
)
{
    i++;
    lc.root = tokens[i];
    i++;
    expectSemicolon(tokens, i);
}

static void parseIndex
(
    std::vector<std::string>&	tokens,
    size_t&						i,
    parse::locConfig&			lc
)
{
    i++;
    lc.index = tokens[i];
    i++;
    expectSemicolon(tokens, i);
}

static void parseAutoindex
(
    std::vector<std::string>&	tokens,
    size_t&						i,
    parse::locConfig&			lc
)
{
    i++;
    lc.autoindex = (tokens[i] == "on");
    i++;
    expectSemicolon(tokens, i);
}

static void parseUploadStore
(
    std::vector<std::string>&	tokens,
    size_t&						i,
    parse::locConfig&			lc
)
{
    i++;
    lc.uploadStore = tokens[i];
    i++;
    expectSemicolon(tokens, i);
}

static void parseCgiPass
(
    std::vector<std::string>&	tokens,
    size_t&						i,
    parse::locConfig&			lc
)
{
    i++;
    lc.cgiPass = tokens[i];
    i++;
    expectSemicolon(tokens, i);
}

static void parseReturn
(
    std::vector<std::string>&	tokens,
    size_t&						i,
    parse::locConfig&			lc
)
{
    i++;
    lc.redirectCode = std::atoi(tokens[i].c_str());
    i++;
    lc.redirectUrl = tokens[i];
    i++;
    expectSemicolon(tokens, i);
}

void parseLocationBlock
(
    std::vector<std::string>&	tokens,
    size_t&						i,
    parse::locConfig&			lc
)
{
    while (i < tokens.size() && tokens[i] != "}")
    {
        if (tokens[i] == "methods")
            parseMethods(tokens, i, lc);
        else if (tokens[i] == "root")
            parseRoot(tokens, i, lc);
        else if (tokens[i] == "index")
            parseIndex(tokens, i, lc);
        else if (tokens[i] == "autoindex")
            parseAutoindex(tokens, i, lc);
        else if (tokens[i] == "upload_store")
            parseUploadStore(tokens, i, lc);
        else if (tokens[i] == "cgi_pass")
            parseCgiPass(tokens, i, lc);
        else if (tokens[i] == "return")
            parseReturn(tokens, i, lc);
        else
            throw std::runtime_error("unknown directive in location block: " + tokens[i]);
    }
    if (i >= tokens.size())
        throw std::runtime_error("expected '}' to close location block");
    i++;
}
