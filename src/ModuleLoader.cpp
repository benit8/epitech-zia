/*
** EPITECH PROJECT, 2019
** zia
** File description:
** ModuleLoader.cpp
*/

#include "ModuleLoader.hpp"

////////////////////////////////////////////////////////////////////////////////

ModuleLoader::ModuleLoader(const std::string &modulesPath)
: m_modulesPath(modulesPath)
{
}

ModuleLoader::~ModuleLoader()
{
	for (auto it = m_handles.begin(); it != m_handles.end(); ++it) {
		void (*destructor)(void *);
		*(void **)(&destructor) = dlsym(it->second, "unloadModule");

		(*destructor)(m_modules[it->first]);

		dlclose(it->second);
	}
}

////////////////////////////////////////////////////////////////////////////////

IModule *ModuleLoader::loadModule(const std::string &moduleName)
{
	// Already loaded ?
	auto it = m_modules.find(moduleName);
	if (it != m_modules.end())
		return it->second;

	// Load DL
	std::string modulePath = makeModulePath(moduleName);
	void *handle = dlopen(modulePath.c_str(), RTLD_LAZY);
	if (!handle)
		throw std::runtime_error(std::string(dlerror()));

	m_handles.emplace(moduleName, handle);

	// Load constructor
	IModule *(*constructor)();
	*(void **)(&constructor) = dlsym(handle, "loadModule");
	if (dlerror())
		throw std::runtime_error("Constructor not found.\n" + std::string(dlerror()));

	IModule *mod = (*constructor)();
	m_modules.emplace(moduleName, mod);

	return mod;
}

std::string ModuleLoader::makeModulePath(const std::string &moduleName)
{
	std::string dlName = moduleName;
	std::transform(dlName.begin(), dlName.end(), dlName.begin(), ::tolower);
	return m_modulesPath + "/libmod_" + dlName + MODULE_EXTENSION;
}

void ModuleLoader::setModulesPath(const std::string &modulesPath)
{
	if (modulesPath != "")
		m_modulesPath = modulesPath;
}