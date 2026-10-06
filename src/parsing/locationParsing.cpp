/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationParsing.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 18:14:09 by akoaik            #+#    #+#             */
/*   Updated: 2026/09/07 11:30:50 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "classes/imports.hpp"

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
        const std::string &m = tokens[i];
        if (m != "GET" && m != "POST" && m != "DELETE")
            throw std::runtime_error("invalid method: " + m);
        lc.methods.push_back(m);
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
    if (i >= tokens.size())
        throw std::runtime_error("unexpected end of file after 'root'");
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
    if (i >= tokens.size())
        throw std::runtime_error("unexpected end of file after 'index'");
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
    if (i >= tokens.size())
        throw std::runtime_error("unexpected end of file after 'autoindex'");
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
    if (i >= tokens.size())
        throw std::runtime_error("unexpected end of file after 'upload_store'");
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
    if (i >= tokens.size())
        throw std::runtime_error("unexpected end of file after 'cgi_pass'");
    lc.cgiPass = tokens[i];
    i++;
    if (i < tokens.size() && tokens[i] != ";")
    {
        lc.cgiInterpreter = tokens[i];
        i++;
    }
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
    if (i >= tokens.size())
        throw std::runtime_error("unexpected end of file after 'return'");
    lc.redirectCode = std::atoi(tokens[i].c_str());
    i++;
    if (i >= tokens.size())
        throw std::runtime_error("unexpected end of file: expected URL after redirect code");
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
