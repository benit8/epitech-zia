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
#include <string>

////////////////////////////////////////////////////////////////////////////////

class Zia
{
public:
	Zia(uint16_t port);
	~Zia();

	static void usage();

private:
	uint16_t m_port;
};