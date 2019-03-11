/*
** EPITECH PROJECT, 2019
** zia
** File description:
** ModuleLoader.cpp
*/

#include "ModuleLoader.hpp"
#include "Logger.hpp"

////////////////////////////////////////////////////////////////////////////////

ModuleLoader::ModuleLoader(const std::string &modulesPath)
: m_modulesPath(modulesPath)
{
}

ModuleLoader::~ModuleLoader()
{
	for (auto &mod : m_mods) {
		void (*destructor)(void *);
		*(void **)(&destructor) = dlsym(mod.second.handle, "unloadModule");
		(*destructor)(mod.second.module);

		dlclose(mod.second.handle);
		Logger::info() << "Unloaded module '" << mod.first << "'" << std::endl;
	}
}

////////////////////////////////////////////////////////////////////////////////

IModule *ModuleLoader::loadModule(const std::string &moduleName)
{
	// Already loaded ?
	auto it = m_mods.find(moduleName);
	if (it != m_mods.end())
		return it->second.module;

	// Load DL
	std::string modulePath = makeModulePath(moduleName);
	void *handle = dlopen(modulePath.c_str(), RTLD_LAZY);
	if (!handle)
		throw std::runtime_error(std::string(dlerror()));

	// Load constructor
	IModule *(*constructor)(ModuleLoader *);
	*(void **)(&constructor) = dlsym(handle, "loadModule");
	char *e = dlerror();
	if (constructor == NULL || e)
		throw std::runtime_error("Constructor not found.\n" + std::string(e));

	IModule *mod = (*constructor)(this);
	ModuleContainer c{handle, mod};
	m_mods.emplace(moduleName, c);

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

IModule *ModuleLoader::getModule(const std::string &name)
{
	return m_mods.find(name) != m_mods.end() ? m_mods[name].module : nullptr;
}