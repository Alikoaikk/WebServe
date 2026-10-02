/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_helper.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 01:57:26 by msafa             #+#    #+#             */
/*   Updated: 2026/10/03 01:57:31 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"
#include "classes/Request_helper.hpp"

static int hexValue(char c)
{
    if(c >= '0' && c <= '9')
        return c - '0';
    if(c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if(c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

bool urlDecode(const std::string& in, std::string& out)
{
    out.clear();
    for(size_t i = 0; i < in.size(); i++)
    {
        if(in[i] != '%')
        {
            out += in[i];
            continue;
        }
        if(i + 2 >= in.size())
            return false;
        int hi = hexValue(in[i + 1]);
        int lo = hexValue(in[i + 2]);
        if(hi == -1 || lo == -1)
            return false;
        char c = static_cast<char>(hi * 16 + lo);
        if(c == '\0')
            return false;
        out += c;
        i += 2;
    }
    return true;
}

bool normalizeUri(const std::string& in, std::string& out)
{
    if (in.empty() || in[0] != '/')
        return false;
    std::vector<std::string> segments;
    size_t start = 0;
    while(start <= in.size())
    {
        size_t slash = in.find('/', start);
        if(slash == std::string::npos)
            slash = in.size();
        std::string seg = in.substr(start, slash - start);
        if(seg == "..")
        {
            if(segments.empty())
                return false;
            segments.pop_back();
        }
        else if(!seg.empty() && seg != ".")
            segments.push_back(seg);
        start = slash + 1;
    }
    bool endsAsDir = in[in.size() - 1] == '/'
        || (in.size() >= 2 && in.substr(in.size() - 2) == "/.")
        || (in.size() >= 3 && in.substr(in.size() - 3) == "/..");
    out = "/";
    for (size_t i = 0; i < segments.size(); i++)
    {
        if(i > 0)
            out += "/";
        out += segments[i];
    }
    if (endsAsDir && !segments.empty())
        out += "/";
    return true;
}
