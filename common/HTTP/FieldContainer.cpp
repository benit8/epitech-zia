/*
** EPITECH PROJECT, 2019
** zia
** File description:
** FieldContainer.cpp
*/

#include "FieldContainer.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace HTTP
{

////////////////////////////////////////////////////////////////////////////////

void FieldContainer::parseFieldData(const std::string &data)
{
	std::istringstream iss(data);
	std::string line;
	while (std::getline(iss, line)) {
		const std::regex reg("^([A-Za-z\\-]+)\\s*:\\s*(.*)$");
		std::smatch match;
		if (std::regex_search(line, match, reg) && match.size() == 3)
			m_fields[match[1]] = match[2];
	}
}

////////////////////////////////////////////////////////////////////////////////

}