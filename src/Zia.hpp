/*
** EPITECH PROJECT, 2019
** zia
** File description:
** Zia.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "json.hpp"
#include "ModuleLoader.hpp"
#include "Network/SocketSelector.hpp"

#include <fstream>
#include <iostream>
#include <list>
#include <string>

using json = nlohmann::json;

////////////////////////////////////////////////////////////////////////////////

class Zia
{
public:
	Zia(const std::string &configFilename);
	~Zia();

public:
	void loadConfig();
	void loadModules();
	int run();

private:
	std::string m_configFilename;
	json m_config;

	ModuleLoader m_moduleLoader;
	Net::SocketSelector m_selector;
  //  	std::map<std::string, std::vector<Net::TcpListener>> m_hosts;
};
