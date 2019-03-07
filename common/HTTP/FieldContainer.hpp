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
	bool hasField(const std::string &key) { return m_fields.find(key) != m_fields.end(); }
	void clearFields() { m_fields.clear(); }
	void setField(const std::string &key, const std::string &value) { m_fields[key] = value; }
	void unsetField(const std::string &key) { m_fields.erase(key); }
	const std::string &getField(const std::string &key) { return m_fields[key]; }
	void defaultField(const std::string &key, const std::string &value) { if (!hasField(key)) setField(key, value); }

	std::string operator [](const std::string &key) { return m_fields[key]; }

protected:
	void parseFields(std::istringstream &iss);
	void exportFields(std::ostringstream &oss);

protected:
	std::map<std::string, std::string> m_fields;
};

////////////////////////////////////////////////////////////////////////////////

}