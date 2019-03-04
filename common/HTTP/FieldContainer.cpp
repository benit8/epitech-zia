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

void FieldContainer::parseFields(const std::string &data)
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

std::string FieldContainer::exportFields()
{
	std::ostringstream oss;

	for (auto field : m_fields) {
		oss << field.first << ": " << field.second << "\r\n";
	}

	return oss.str();
}

////////////////////////////////////////////////////////////////////////////////

}