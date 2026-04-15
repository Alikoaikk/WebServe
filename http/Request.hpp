/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 18:31:56 by msafa             #+#    #+#             */
/*   Updated: 2026/04/15 14:54:57 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <string>

enum ParseState
{
    PARSE_REQUEST_LINE,
    PARSE_HEADERS,
    PARSE_BODY,
    PARSE_COMPLETE,
    PARSE_ERROR
};

class Request
{
    public:
        std::string _method;
        std::string _uri;
        std::string _version;
        std::map<std::string,std::string> _headers;
        std::string _body;
        std::string _queryString;
        std::string _path;
        ParseState _parseState;
        Request();
        ~Request();
        void parse(std::string& chunk);
    private:
        std::string _rawBuffer;
        size_t _contentLength;
        bool _chunked;
};

#endif