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

void FieldContainer::parseFields(std::istream &is)
{
	const std::regex reg("^([A-Za-z\\-]+)\\s*:\\s*(.*)\\r$");

	std::string line;
	while (std::getline(is, line)) {
		std::smatch match;
		if (!std::regex_search(line, match, reg))
			break;
		if (match.size() == 3)
			m_fields[match[1]] = match[2];
	}
}

void FieldContainer::exportFields(std::ostream &os)
{
	for (auto field : m_fields) {
		os << field.first << ": " << field.second << "\r\n";
	}
}

////////////////////////////////////////////////////////////////////////////////

}