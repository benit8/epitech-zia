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
: AModule(ml)
{
}

PHPCGI::~PHPCGI()
{
}

////////////////////////////////////////////////////////////////////////////////

bool PHPCGI::onContentGen(json &host, HTTP::Request &req, HTTP::Response &res)
{
	int pipes[2];
	pipe(pipes);

	pid_t pid = fork();
	if (pid < 0) {
		Logger::error() << "PHPCGI::onContentGen(): fork() failed" << std::endl;
		close(pipes[0]);
		close(pipes[1]);
		return false;
	}
	else if (pid == 0) {
		// child
		const char *bin = "/usr/bin/php-cgi";
		const char *argv[] = {bin, NULL};
		const char **env = buildEnv(host, req);
		close(pipes[0]);
		dup2(pipes[1], 1);
		int rc = execve(
			bin,
			const_cast<char * const *>(argv),
			const_cast<char * const *>(env)
		);
		for (size_t i = 0; env[i] != NULL; ++i)
			delete env[i];
		delete env;
		exit(rc);
	}
	else {
		close(pipes[1]);

		int exitCode = 0;
		waitpid(pid, &exitCode, 0);
		if (exitCode != 0) {
			Logger::error() << "PHPCGI::onContentGen(): got exit code " << exitCode << std::endl;
		}

		int r = -1;
		char buff[128];
		std::string content;
		while ((r = read(pipes[0], buff, 128)) != -1 && r > 0)
			content.append(buff, r);
		close(pipes[0]);
		if (r == -1) {
			Logger::error() << "PHPCGI::onContentGen(): read() failed" << std::endl;
			return false;
		}

		std::istringstream iss(content);
		res.parseFields(iss);
		if (!res.hasField("Status"))
			res.status(HTTP::Response::Ok);
		res.body(iss);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

const char **PHPCGI::buildEnv(json &host, HTTP::Request &req)
{
	std::map<std::string, std::string> env;

	for (auto p : req) {
		std::string s = p.first;
		if (s.find("__") == 0)
			continue;
		std::replace(s.begin(), s.end(), '-', '_');
		std::transform(s.begin(), s.end(), s.begin(), ::toupper);
		s = "HTTP_" + s;
		env[s] = p.second;
	}

	env["DOCUMENT_ROOT"] = host["Root"];
	env["GATEWAY_INTERFACE"] = "CGI/1.1";
	env["PATH"] = std::getenv("PATH");
	env["PATH_INFO"] = req.uri();
	env["PATH_TRANSLATED"] = fs::system_complete(req["__scriptName"]);
	env["PATHEXT"] = ".COM;.EXE;.BAT;.CMD;.VBS;.VBE;.JS;.JSE;.WSF;.WSH;.MSC";
	env["QUERY_STRING"] = req.query();
	env["REDIRECT_STATUS"] = "true";
	env["REMOTE_ADDR"] = req["__remoteAddress"];
	env["REMOTE_PORT"] = req["__remotePort"];
	env["REQUEST_METHOD"] = req.method();
	env["REQUEST_URI"] = req.uri();
	if (!req.query().empty())
		env["REQUEST_URI"] += "?" + req.query();
	env["SCRIPT_FILENAME"] = fs::system_complete(req["__scriptName"]);
	env["SCRIPT_NAME"] = req["__scriptName"];
	env["SERVER_ADDR"] = Net::IpAddress(req.host()).toString();
	env["SERVER_ADMIN"] = "contact@a.zia";
	env["SERVER_NAME"] = req.host();
	env["SERVER_PORT"] = req.port();
	env["SERVER_PROTOCOL"] = req.version();
	env["SERVER_SIGNATURE"] = "<address>Zia/0.1 Server at " + env["SERVER_NAME"] + " at Port " + env["SERVER_PORT"] + "</address>\n";
	env["SERVER_SOFTWARE"] = "Zia/0.1";
	env["SYSTEMROOT"] = fs::current_path().root_name();

	struct timeval tp;
	gettimeofday(&tp, NULL);
	env["REQUEST_TIME"] = std::to_string(tp.tv_sec);
	env["REQUEST_TIME_FLOAT"] = std::to_string(tp.tv_sec * 1000 + tp.tv_usec / 1000);

	size_t i = 0;
	const char **e = new const char *[env.size() + 1];
	for (auto it = env.begin(); it != env.end(); ++it) {
		std::string s = it->first + "=" + it->second;
		int size = s.size();
		char *buff = new char[size + 1];
		s.copy(buff, size);
		buff[size] = '\0';
		e[i++] = buff;
	}
	e[i] = NULL;
	return e;
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


// 'COMSPEC' => string 'C:\WINDOWS\system32\cmd.exe' (length=27)
// 'CONTEXT_DOCUMENT_ROOT' => string 'C:/wamp/www' (length=11)
// 'CONTEXT_PREFIX' => string '' (length=0)
// 'DOCUMENT_ROOT' => string 'C:/wamp/www' (length=11)
// 'GATEWAY_INTERFACE' => string 'CGI/1.1' (length=7)
// 'HTTP_ACCEPT' => string 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3' (length=118)
// 'HTTP_ACCEPT_ENCODING' => string 'gzip, deflate, br' (length=17)
// 'HTTP_ACCEPT_LANGUAGE' => string 'fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7,ja;q=0.6,pt;q=0.5,lb;q=0.4' (length=62)
// 'HTTP_CACHE_CONTROL' => string 'max-age=0' (length=9)
// 'HTTP_CONNECTION' => string 'keep-alive' (length=10)
// 'HTTP_COOKIE' => string 'PHPSESSID=ceg5urnbqfrlg9v56mkifum1um' (length=36)
// 'HTTP_DNT' => string '1' (length=1)
// 'HTTP_HOST' => string 'localhost' (length=9)
// 'HTTP_UPGRADE_INSECURE_REQUESTS' => string '1' (length=1)
// 'HTTP_USER_AGENT' => string 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.75 Safari/537.36' (length=114)
// 'PATH' => string 'C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\Program Files (x86)\Intel\iCLS Client\;C:\Program Files\Intel\iCLS Client\;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files\Intel\Intel(R) Management Engine Components\IPT;C:\Pr'... (length=767)
// 'PATHEXT' => string '.COM;.EXE;.BAT;.CMD;.VBS;.VBE;.JS;.JSE;.WSF;.WSH;.MSC' (length=53)
// 'PHP_SELF' => string '/Benito.io/index.php' (length=20)
// 'QUERY_STRING' => string 'p=o' (length=3)
// 'REMOTE_ADDR' => string '::1' (length=3)
// 'REMOTE_PORT' => string '55324' (length=5)
// 'REQUEST_METHOD' => string 'GET' (length=3)
// 'REQUEST_SCHEME' => string 'http' (length=4)
// 'REQUEST_TIME' => int 1552740523
// 'REQUEST_TIME_FLOAT' => float 1552740523.198
// 'REQUEST_URI' => string '/Benito.io/?p=o' (length=15)
// 'SCRIPT_FILENAME' => string 'C:/wamp/www/Benito.io/index.php' (length=31)
// 'SCRIPT_NAME' => string '/Benito.io/index.php' (length=20)
// 'SERVER_ADDR' => string '::1' (length=3)
// 'SERVER_ADMIN' => string 'wampserver@wampserver.invalid' (length=29)
// 'SERVER_NAME' => string 'localhost' (length=9)
// 'SERVER_PORT' => string '80' (length=2)
// 'SERVER_PROTOCOL' => string 'HTTP/1.1' (length=8)
// 'SERVER_SIGNATURE' => string '<address>Apache/2.4.35 (Win64) PHP/7.2.10 Server at localhost Port 80</address>' (length=80)
// 'SERVER_SOFTWARE' => string 'Apache/2.4.35 (Win64) PHP/7.2.10' (length=32)
// 'SystemRoot' => string 'C:\WINDOWS' (length=10)
// 'WINDIR' => string 'C:\WINDOWS' (length=10)