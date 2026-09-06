/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiBuildResponse.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 21:18:25 by akoaik            #+#    #+#             */
/*   Updated: 2026/09/06 21:50:50 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"

Response cgiBuildResponse(const Request& req, const parse::locConfig& loc, const std::string& fullPath)
{
    Response res;

    

    return res;
}


/*
     Runs the CGI script at `fullPath` and turns its output into a Response.

     Steps to implement (fill in the bodies below):
       1. Build the CGI environment (char** envp):
            REQUEST_METHOD, CONTENT_LENGTH, CONTENT_TYPE, QUERY_STRING,
            SCRIPT_FILENAME (= fullPath), PATH_INFO,
            GATEWAY_INTERFACE=CGI/1.1, SERVER_PROTOCOL=HTTP/1.1
       2. Create two pipes: one for the child's stdin (request body),
          one for the child's stdout (CGI output).
       3. fork().
            - child : dup2 the pipes onto STDIN/STDOUT, chdir into the
                      script's directory, execve(interpreter, args, envp).
            - parent: write req body to child stdin, close it (EOF = end of
                      body), read child stdout until EOF.
       4. waitpid the child.
       5. Parse CGI output: headers, blank line (\r\n\r\n), then body.
          Feed headers into the Response, the rest into the body.
*/
