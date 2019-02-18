/*
** EPITECH PROJECT, 2019
** zia
** File description:
** ModuleLoader.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

class ModuleLoader;

////////////////////////////////////////////////////////////////////////////////

#include "Module.hpp"

#include <algorithm>
#include <dlfcn.h>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
	#define MODULE_EXTENSION ".dll"
#else
	#define MODULE_EXTENSION ".so"
#endif

////////////////////////////////////////////////////////////////////////////////

class ModuleLoader
{
public:
	ModuleLoader(const std::string &modulesPath = "./modules");
	~ModuleLoader();

	IModule *loadModule(const std::string &moduleName);
	void setModulesPath(const std::string &modulesPath);

private:
	std::string makeModulePath(const std::string &moduleName);

private:
	std::string m_modulesPath;
	std::map<std::string, void *> m_handles;
	std::map<std::string, IModule *> m_modules;
};