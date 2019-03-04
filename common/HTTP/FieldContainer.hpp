/*
** EPITECH PROJECT, 2019
** zia
** File description:
** FieldContainer.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace HTTP {
	class FieldContainer;
}

////////////////////////////////////////////////////////////////////////////////

#include <map>
#include <regex>
#include <string>

////////////////////////////////////////////////////////////////////////////////

namespace HTTP
{

////////////////////////////////////////////////////////////////////////////////

class FieldContainer
{
public:
	virtual ~FieldContainer() = default;

public:
	void clearFields() { m_fields.clear(); }
	void setField(const std::string &key, const std::string &value) { m_fields[key] = value; }
	void unsetField(const std::string &key) { m_fields.erase(key); }
	const std::string &getField(const std::string &key) { return m_fields[key]; }

	std::string operator [](const std::string &key) { return m_fields[key]; }

public:
	void parseFields(const std::string &data);
	std::string exportFields();

protected:
	std::map<std::string, std::string> m_fields;
};

////////////////////////////////////////////////////////////////////////////////

}