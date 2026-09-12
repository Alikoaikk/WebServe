/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiBuildResponse.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 21:18:25 by akoaik            #+#    #+#             */
/*   Updated: 2026/09/13 01:40:56 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"
#include <sys/wait.h>

static void buildEnv
(
    std::vector<std::string>&	env,
    const Request&				req,
    const std::string&			fullPath
)
{
    std::ostringstream oss;
    oss << req._body.size();

    env.push_back("REQUEST_METHOD=" + req._method);
    env.push_back("QUERY_STRING=" + req._queryString);
    env.push_back("CONTENT_LENGTH=" + oss.str());
    env.push_back("SCRIPT_FILENAME=" + fullPath);
    env.push_back("PATH_INFO=");
    env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    env.push_back("SERVER_PROTOCOL=HTTP/1.1");
}

Response cgiBuildResponse(const Request& req, const parse::locConfig& loc, const std::string& fullPath)
{
    Response res;
    int inPipe[2];
    int outPipe[2];


    std::vector<std::string> env;
    buildEnv(env, req, fullPath);

    if (pipe(inPipe )== -1)
    {
        res.setStatusCode(500);
        return res ;
    }
    if (pipe(outPipe) == -1)
    {
        close(inPipe[0]);
        close(inPipe[1]);
        res.setStatusCode(500);
        return res;
    }

	int pid = fork();
	if (pid == -1)
	{
		close(inPipe[0]);
		close(inPipe[1]);
		close(outPipe[0]);
		close(outPipe[1]);
		res.setStatusCode(500);
		return res;
	}
	else if (pid == 0)
	{
		dup2(inPipe[0], 0);
		dup2(outPipe[1], 1);

		close(inPipe[0]);
		close(inPipe[1]);
		close(outPipe[0]);
		close(outPipe[1]);

		/*

		 	   std::string dir = fullPath;
       79 +    std::string script = fullPath;
       80 +    size_t slash = fullPath.find_last_of('/');
       81 +    if (slash != std::string::npos)
       82 +    {
       83 +      dir = fullPath.substr(0, slash);
       84 +      script = fullPath.substr(slash + 1);
       85 +      chdir(dir.c_str());
       86 +    }
       87 +
       88 +    std::string interpreter;
       89 +    if (loc.cgiPass == ".py")
       90 +      interpreter = "/usr/bin/python3";
       91 +    else if (loc.cgiPass == ".php")
       92 +      interpreter = "/usr/bin/php-cgi";
       93 +
       94 +    char* argv[3];
       95 +    argv[0] = const_cast<char*>(interpreter.c_str());
       96 +    argv[1] = const_cast<char*>(script.c_str());
       97 +    argv[2] = NULL;
       98 +
       99 +    std::vector<char*> envp;
      100 +    for (size_t i = 0; i < env.size(); ++i)
      101 +      envp.push_back(const_cast<char*>(env[i].c_str()));
      102 +    envp.push_back(NULL);
      103 +
      104 +    execve(argv[0], argv, &envp[0]);
      103 +
      104 +    execve(argv[0], argv, &envp[0]);
      105 +    exit(1);

		*/
	}
	else
	{

	}




    return res;
}
