/*
** EPITECH PROJECT, 2019
** zia
** File description:
** Zia.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Config/INIConfig.hpp"

#include <iostream>
#include <list>
#include <string>

////////////////////////////////////////////////////////////////////////////////

class Zia
{
public:
	Zia();
	~Zia();

public:
	void loadConfig();
	void loadModules();
	int run();

private:
	void loadDefaultConfig();

protected:
	static const std::string configFilename;

private:
	INIConfig m_config;
	uint16_t m_port;
};