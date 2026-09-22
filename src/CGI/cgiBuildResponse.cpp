/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiBuildResponse.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 21:18:25 by akoaik            #+#    #+#             */
/*   Updated: 2026/09/23 00:43:07 by akoaik           ###   ########.fr       */
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

static std::string getInterpreter(const parse::locConfig& loc)
{
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
        _exit(1);
}

static int runCgiChild
(
	int					pid,
	int					inPipe[2],
	int					outPipe[2],
	const std::string&	body,
	std::string&		output
)
{
	close(inPipe[0]);
	close(outPipe[1]);

	if (!body.empty())
		write(inPipe[1], body.c_str(), body.size());
	close(inPipe[1]);

	char buf[4096];
	ssize_t n = read(outPipe[0], buf, sizeof(buf));
	while (n > 0)
	{
		output.append(buf, n);
		n = read(outPipe[0], buf, sizeof(buf));
	}
	close(outPipe[0]);

	int status;
	waitpid(pid, &status, 0);
	return status;
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
    if(pipe(outPipe) == -1)
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

		std::string interpreter = getInterpreter(loc);
		std::string script = fullPath;

		size_t slash = fullPath.find_last_of('/');
		if(slash != std::string::npos)
		{
			chdir(fullPath.substr(0, slash).c_str());
			script = fullPath.substr(slash + 1);
		}

		execCgi(interpreter, script, env);
	}
	else
	{
		std::string output;
		int status = runCgiChild(pid, inPipe, outPipe, req._body, output);

		if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
		{
			res.setStatusCode(500);
			return res;
		}
		res.setStatusCode(200);
		res.setBody(output);
	}
    return res;
}
