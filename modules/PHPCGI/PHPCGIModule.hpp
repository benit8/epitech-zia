/*
** EPITECH PROJECT, 2019
** zia
** File description:
** PHPCGIModule.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace Modules {
	class PHPCGI;
}

////////////////////////////////////////////////////////////////////////////////

#include "Module.hpp"

#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <experimental/filesystem>
#include <string>

namespace fs = std::experimental::filesystem;

////////////////////////////////////////////////////////////////////////////////

namespace Modules
{

	class EXPORT PHPCGI : public AModule
	{
	public:
		PHPCGI(ModuleLoader *ml);
		~PHPCGI();

		bool onContentGen(json &host, HTTP::Request &req, HTTP::Response &res) override;

	private:
		const char **buildEnv(json &host, HTTP::Request &req);
	};

}