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

bool urlDecode(std::string& uri)
{
    std::string out;
    for(size_t i = 0; i < uri.size(); i++)
    {
        if(uri[i] != '%')
        {
            out += uri[i];
            continue;
        }
        if(i + 2 >= uri.size())
            return false;
        int hi = hexValue(uri[i + 1]);
        int lo = hexValue(uri[i + 2]);
        if(hi == -1 || lo == -1)
            return false;
        char c = static_cast<char>(hi * 16 + lo);
        if(c == '\0')
            return false;
        out += c;
        i += 2;
    }
    uri = out;
    return true;
}

static bool resolveSegments(const std::string& uri, std::vector<std::string>& segments)
{
    size_t start = 0;
    while(start <= uri.size())
    {
        size_t slash = uri.find('/', start);
        if(slash == std::string::npos)
            slash = uri.size();
        std::string seg = uri.substr(start, slash - start);
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
    return true;
}

static std::string joinSegments(const std::vector<std::string>& segments, bool endsAsDir)
{
    std::string out = "/";
    for (size_t i = 0; i < segments.size(); i++)
    {
        if(i > 0)
            out += "/";
        out += segments[i];
    }
    if (endsAsDir && !segments.empty())
        out += "/";
    return out;
}

bool normalizeUri(std::string& uri)
{
    if (uri.empty() || uri[0] != '/')
        return false;
    std::vector<std::string> segments;
    if (!resolveSegments(uri, segments))
        return false;
    bool endsAsDir = uri[uri.size() - 1] == '/'
        || (uri.size() >= 2 && uri.substr(uri.size() - 2) == "/.")
        || (uri.size() >= 3 && uri.substr(uri.size() - 3) == "/..");
    uri = joinSegments(segments, endsAsDir);
    return true;
}
