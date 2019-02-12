/*
** EPITECH PROJECT, 2019
** zia
** File description:
** main.cpp
*/

#include "Zia.hpp"

int main(/*int argc, char **argv*/)
{
	Zia zia;
	zia.loadConfig();
	zia.loadModules();
	return zia.run();
}