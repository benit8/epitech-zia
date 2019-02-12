/*
** EPITECH PROJECT, 2019
** zia
** File description:
** Zia.cpp
*/

#include "Zia.hpp"

////////////////////////////////////////////////////////////////////////////////

const std::string Zia::configFilename = "./zia.conf";

////////////////////////////////////////////////////////////////////////////////

Zia::Zia()
: m_port(8080)
{}

Zia::~Zia()
{}

////////////////////////////////////////////////////////////////////////////////

void Zia::loadConfig()
{
	m_config.loadFromFile(configFilename);
}

void Zia::loadModules()
{
	std::vector<std::string> moduleNames = m_config.getArray<>("zia", "modules");

	for (auto it = moduleNames.begin(); it != moduleNames.end(); ++it) {
		std::cout << *it << std::endl;
	}
}

int Zia::run()
{
	return 0;
}