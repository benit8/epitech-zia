/*
** EPITECH PROJECT, 2019
** zia
** File description:
** Zia.cpp
*/

#include "Zia.hpp"

////////////////////////////////////////////////////////////////////////////////

Zia::Zia(const std::string &configFilename)
: m_configFilename(configFilename)
{
}

Zia::~Zia()
{
}

////////////////////////////////////////////////////////////////////////////////

void Zia::loadConfig()
{
	std::ifstream ifs(m_configFilename);
	if (!ifs.is_open())
		std::cerr << "Failed to open configuration file '" << m_configFilename << "'" << std::endl;
	else
		ifs >> m_config;
}

void Zia::loadModules()
{
	m_moduleLoader.setModulesPath(m_config["modules"]["path"].get<std::string>());

	auto moduleNames = m_config["modules"]["list"].get<std::vector<std::string>>();
	for (auto it = moduleNames.begin(); it != moduleNames.end(); ++it) {
		try {
			m_moduleLoader.loadModule(*it);
			std::cout << "Module '" << *it << "' loaded" << std::endl;
		}
		catch (std::runtime_error &e) {
			std::cerr << "Failed to load module '" << *it << "': " << e.what() << std::endl;
		}
	}
}

int Zia::run()
{
	// Parse website files ?
	// Get a compilation of all the ports we need to listen to (eg. 80, 443, ...)
	// Boot a TcpListener for each of them, add it to the selector

	for (auto& host : m_config["hosts"]) {
		std::cout << host["name"].get<std::string>() << std::endl;
		for (auto& address : host["address"]) {
			std::cout << "\t" << address.get<std::string>() << std::endl;
		}
	}

	return 0;
}