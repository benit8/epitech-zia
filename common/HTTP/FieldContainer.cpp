/*
** EPITECH PROJECT, 2019
** zia
** File description:
** FieldContainer.cpp
*/

#include "FieldContainer.hpp"
#include <iostream>
////////////////////////////////////////////////////////////////////////////////

namespace HTTP
{

////////////////////////////////////////////////////////////////////////////////

void FieldContainer::parseFields(std::istringstream &iss)
{
	const std::regex reg("^([A-Za-z\\-]+)\\s*:\\s*(.*)\\r$");

	std::string line;
	while (std::getline(iss, line)) {
		std::smatch match;
		if (std::regex_search(line, match, reg) && match.size() == 3)
			m_fields[match[1]] = match[2];
	}
}

void FieldContainer::exportFields(std::ostringstream &oss)
{
	for (auto field : m_fields) {
		oss << field.first << ": " << field.second << "\r\n";
	}
}

////////////////////////////////////////////////////////////////////////////////

}