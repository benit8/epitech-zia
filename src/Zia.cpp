/*
** EPITECH PROJECT, 2019
** zia
** File description:
** Zia.cpp
*/

#include "Zia.hpp"

////////////////////////////////////////////////////////////////////////////////

Zia::Zia(uint16_t port)
: m_port(port)
{}

Zia::~Zia()
{}

////////////////////////////////////////////////////////////////////////////////

void Zia::usage()
{
	std::cerr << "USAGE:" << std::endl;
	std::cerr << "\tzia port" << std::endl;
	std::cerr << "DESCRIPTION:" << std::endl;
	std::cerr << "\tport\tthe port you fucking nigger" << std::endl;
}