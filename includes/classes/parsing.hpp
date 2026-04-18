/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 17:06:11 by akoaik            #+#    #+#             */
/*   Updated: 2026/04/15 16:29:13 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
#define PARSING_HPP

#include <string>
#include <vector>
#include <map>
#include <stdexcept>


class parse
{
    public :
        std::string configFileName ;
        typedef struct locationConfig
        {
            std::string                 path ;
            std::vector<std::string>    methods ;
            std::string                 root ;
            std::string                 index ;
            int                         redirectCode ;
            bool                        autoindex ;
            std::string                 uploadStore ;
            std::string                 redirectUrl ;
            std::string                 cgiPass ;
        }	locConfig ;
        typedef struct serverConfig
        {
            int                         port ;
			unsigned int                clientMaxBodySize ;
			std::string					host ;
			std::map<int, std::string>	errorPages ;
			std::vector<locConfig>		locations ;
        }	serConfig ;

        std::vector<serConfig>  servers ;
        parse(std::string configFileName);
        void parseTokens(std::vector<std::string> tokens);
        void parseServerBlock(std::vector<std::string>& tokens, size_t& i, serConfig& sc);
};

std::vector<std::string>    tokenize(const std::string &filename);
void                        expectSemicolon(const std::vector<std::string>& tokens, size_t& i);
unsigned int                parseSize(const std::string& str);
void                        parseLocationBlock(std::vector<std::string>& tokens, size_t& i, parse::locConfig& lc);
void                        printConfig(const parse& p);


#endif

/*

    # parsing structure :

        serconfig :
            host and port       ->  which network interface + port
            clientMaxBodySize   ->  max allowed request body
            errorPages          ->  map of http status code
            locations           ->  all location blocks defined insider the server
                                    example :
                                        path = "/files"
                                        method = [GET]
                                        root = "var/www/files"
        locConfig :
            path                ->  The URL prefix this location matches
            methods             ->  which HTTP methods are allowed (GET, POST, ...)
            root                ->  filesystem directory to serve files from
            index               ->  default file to serve when it is requested
            autoindex           ->  if TRUE, show a directory list when no index file exist
            uploadStore         ->  folder where upload file are saved
            cgiPass             ->  file extention that triggers CGI execution (like .py file)
       redirectCode/redirectUrl -> send an HTT redirect (301 /new)

    Strucutre example :

        servers  (vector of serConfig)
        ├── serConfig  port=8080
        │   └── locations [/, /files, /upload, /old, /cgi-bin]
        └── serConfig  port=8000
            └── locations [/]

*/
