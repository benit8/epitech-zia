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

#include <iostream>
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
	struct ci_less
	{
		struct nocase_compare
		{
			bool operator() (const unsigned char& c1, const unsigned char& c2) const {
				return tolower(c1) < tolower(c2);
			}
		};
		bool operator() (const std::string & s1, const std::string & s2) const {
			return std::lexicographical_compare(
				s1.begin(), s1.end(),
				s2.begin(), s2.end(),
				nocase_compare()
			);
		}
	};

public:
	virtual ~FieldContainer() = default;

public:
	std::map<std::string, std::string>::iterator begin() { return m_fields.begin(); }
	std::map<std::string, std::string>::iterator end() { return m_fields.end(); }

	bool hasField(const std::string &key) { return m_fields.find(key) != m_fields.end(); }
	void clearFields() { m_fields.clear(); }
	void setField(const std::string &key, const std::string &value) { m_fields[key] = value; }
	void unsetField(const std::string &key) { m_fields.erase(key); }
	const std::string &getField(const std::string &key) { return m_fields[key]; }
	void defaultField(const std::string &key, const std::string &value) { if (!hasField(key)) setField(key, value); }

	std::string &operator [](const std::string &key) { return m_fields[key]; }

public:
	void parseFields(std::istream &is);
	void exportFields(std::ostream &os);

protected:
	std::map<std::string, std::string, ci_less> m_fields;
};

////////////////////////////////////////////////////////////////////////////////

}