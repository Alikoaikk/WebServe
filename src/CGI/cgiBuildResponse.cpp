/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiBuildResponse.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 21:18:25 by akoaik            #+#    #+#             */
/*   Updated: 2026/10/02 23:58:35 by msafa            ###   ########.fr       */
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

    std::map<std::string, std::string>::const_iterator it = req._headers.find("Content-Type");
    if (it != req._headers.end())
        env.push_back("CONTENT_TYPE=" + it->second);
}

static std::string getInterpreter(const parse::locConfig& loc)
{
    if(!loc.cgiInterpreter.empty())
        return loc.cgiInterpreter;
    if (loc.cgiPass == ".py")
        return "/usr/bin/python3";
    if (loc.cgiPass == ".php")
        return "/usr/bin/php-cgi";
    return "";
}

static void execCgi
(
	const std::string&			interpreter,
	const std::string&			script,
	std::vector<std::string>&	env
)
{
        std::vector<char*>      argv;
        std::vector<char*>      envp;

        argv.push_back(const_cast<char*>(interpreter.c_str()));
        argv.push_back(const_cast<char*>(script.c_str()));
        argv.push_back(NULL);

        for (size_t i = 0; i < env.size(); ++i)
                envp.push_back(const_cast<char*>(env[i].c_str()));
        envp.push_back(NULL);
        execve(argv[0], &argv[0], &envp[0]);
        std::exit(1);
}

cgi_process *cgiBuildResponse(const Request& req, const parse::locConfig& loc, const std::string& fullPath)
{
    Response res;
    int inPipe[2];
    int outPipe[2];
    
    std::vector<std::string> env;
    buildEnv(env, req, fullPath);

    if (pipe(inPipe )== -1)
        return (NULL);

    if(pipe(outPipe) == -1)
    {
        close(inPipe[0]);
        close(inPipe[1]);
        return (NULL);
    }

	int pid = fork();
	if (pid == -1)
	{
		close(inPipe[0]);
		close(inPipe[1]);
		close(outPipe[0]);
		close(outPipe[1]);
		return (NULL);
	}
	else if (pid == 0)
	{
    	dup2(inPipe[0], 0);
		dup2(outPipe[1], 1);

		close(inPipe[0]);
		close(inPipe[1]);
		close(outPipe[0]);
		close(outPipe[1]);

		std::string interpreter = getInterpreter(loc);
		std::string script = fullPath;

		size_t slash = fullPath.find_last_of('/');
		if(slash != std::string::npos)
		{
			chdir(fullPath.substr(0, slash).c_str());
			script = fullPath.substr(slash + 1);
		}

		execCgi(interpreter, script, env);
        std::exit(1);
	}

    // code :

        close(inPipe[0]);
        close(outPipe[1]);

        fcntl(inPipe[1], F_SETFL, O_NONBLOCK);
        fcntl(outPipe[0], F_SETFL, O_NONBLOCK);

        cgi_process* proc = new cgi_process();
        proc->pid = pid;
        proc->inFd = inPipe[1];
        proc->outFd = outPipe[0];
        proc->body = req._body;
        proc->bodySent = 0;
        proc->startTime = time(NULL);
        proc->done = false;

        return proc ;

}
