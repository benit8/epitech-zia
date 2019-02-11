/*
** EPITECH PROJECT, 2019
** zia
** File description:
** INIConfig.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

class INIConfig
{
public:
	INIConfig();
	~INIConfig();

public:
	bool loadFromFile(const std::string &filename);

	template <typename T = std::string>
	T get(const std::string &sectionName, const std::string &key, T def = T())
	{
		auto section = m_values.find(sectionName);
		if (section == m_values.end())
			return def;

		auto it = section->second.find(key);
		if (it == section->second.end())
			return def;

		std::istringstream iss(it->second);
		T value = def;
		iss >> value;
		return value;
	}

private:
	void error(const std::string &message);
	void cleanUpLine(std::string &s);
	std::string getQuotedText(std::string &line, const std::string &quotes = "\"");
	void parseExpression(std::string &line);
	void parseAssign(std::string &line);

private:
	std::map<std::string, std::map<std::string, std::string>> m_values;
	std::string m_currentFilename;
	std::string m_currentSectionName;
	std::size_t m_currentLineNo;
};