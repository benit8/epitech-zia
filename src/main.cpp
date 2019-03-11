/*
** EPITECH PROJECT, 2019
** zia
** File description:
** main.cpp
*/

#include "Zia.hpp"

int main(int argc, char **argv)
{
	Zia zia(argc == 1 ? "./zia.json" : argv[1]);
	zia.loadConfig();
	zia.loadModules();
	return zia.run();


	// Logger::debug() << "testing " << 1 << " " << 1.23f << std::endl;
	// Logger::debug() << "something" << " " << "else" << std::endl;
	// Logger::error() << "haaaaaa" << "aaaaaaaa" << std::endl;
	// return 0;
}