/*
** EPITECH PROJECT, 2019
** zia
** File description:
** PHPCGIModule.cpp
*/

#include "PHPCGIModule.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace Modules
{

////////////////////////////////////////////////////////////////////////////////

PHPCGI::PHPCGI(ModuleLoader *ml)
: AModule(ml, "PHPCGI")
{
}

PHPCGI::~PHPCGI()
{
}

////////////////////////////////////////////////////////////////////////////////

bool PHPCGI::onContentGen(json &host, HTTP::Request &req, HTTP::Response &res)
{
	// int pipes[2];
	// pipe(pipes);

	// pid_t pid = fork();
	// if (pid < 0) {
	// 	close(pipes[0]);
	// 	close(pipes[1]);
	// 	return false;
	// }
	// else if (pid == 0) {
	// 	// child
	// 	const char *bin = "/usr/bin/php-cgi";
	// 	const char *argv[] = {bin, NULL};
	// 	const char **env = buildEnv(req);
	// 	close(pipes[0]);
	// 	dup2(pipes[1], 1);
	// 	exit(execve(
	// 		bin,
	// 		const_cast<char * const *>(argv),
	// 		const_cast<char * const *>(env)
	// 	));
	// }
	// else {
	// 	close(pipes[1]);

	// 	int exitCode = 0;
	// 	waitpid(pid, &exitCode, 0);
	// 	if (exitCode != 0) {
	// 		close(pipes[0]);
	// 		return false;
	// 	}

	// 	int r = -1;
	// 	char buff[128];
	// 	std::string body;
	// 	while ((r = read(pipes[0], buff, 128)) != -1 && r > 0)
	// 		body.append(buff, r);
	// 	if (r == -1) {
	// 		close(pipes[0]);
	// 		return false;
	// 	}

	// 	// parse php-cgi output
	// }

	return true;
}

////////////////////////////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////////////////////////

extern "C"
{
	IModule *loadModule(ModuleLoader *ml)
	{
		return new Modules::PHPCGI(ml);
	}

	void unloadModule(Modules::PHPCGI *mod)
	{
		delete mod;
	}
}