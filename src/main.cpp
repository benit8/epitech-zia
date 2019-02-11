/*
** EPITECH PROJECT, 2019
** zia
** File description:
** main.cpp
*/

#include "Zia.hpp"

int main(int argc, char **argv)
{
	if (argc != 2) {
		Zia::usage();
		return 1;
	}

	Zia zia(std::atoi(argv[1]));

	INIConfig config;
	if (!config.loadFromFile("test.ini"))
		return 1;

	return 0;
}