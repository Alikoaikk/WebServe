/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 18:33:24 by msafa             #+#    #+#             */
/*   Updated: 2026/09/03 18:58:49 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Parsing.hpp"
#include <string>
#include <map>
#include "Parsing.hpp"

class Request;
class parse;

class Response
{
    private:
        std::string							_version;
        int									_statusCode;
        std::string							_statusMessage;
        std::map<std::string, std::string> _header;
        std::string							_body;
    public:
        Response();
        ~Response();
        void setStatusCode(int code);
        int getStatusCode() const;
        const std::string& getBody() const;
        std::string getStatusMessage(int code);
        void setHeader(const std::string& key, const std::string& value);
        void setBody(const std::string& content);
        std::string build();
};


Response handleGet(const Request& req, const parse::serConfig& serv);
Response handlePost(const Request& req, const parse::serConfig& serv);
Response handleDelete(const Request& req, const parse::serConfig& serv);

std::string createPath(const std::string& uri, const parse::locConfig& loc);
std::string getMimeType(const std::string& path);
const parse::locConfig* findLocation(const std::string& uri, const parse::serConfig& serv);

#endif

/*
HTTP Response structure (always the same format):

[Status Line] contains the version + status code+ status message
[Headers] same as the request
[Blank Line] \r\n
[Body] same body

*/
