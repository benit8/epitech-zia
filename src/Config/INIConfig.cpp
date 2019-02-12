/*
** EPITECH PROJECT, 2019
** zia
** File description:
** INIConfig.cpp
*/

#include <algorithm>
#include "INIConfig.hpp"

////////////////////////////////////////////////////////////////////////////////

INIConfig::INIConfig()
: m_currentFilename("")
, m_currentSectionName("")
, m_currentLineNo(0)
{}

INIConfig::~INIConfig()
{}

////////////////////////////////////////////////////////////////////////////////

bool INIConfig::loadFromFile(const std::string &filename)
{
	m_currentFilename = filename;

	std::ifstream ifs(filename);
	if (!ifs.is_open()) {
		std::cerr << "Failed to open ini file '" << filename << "'" << std::endl;
		return false;
	}

	std::string line;
	for (m_currentLineNo = 1; std::getline(ifs, line); ++m_currentLineNo) {
		cleanUpLine(line);
		if (line.empty())
			continue;

		parseExpression(line);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

void INIConfig::error(const std::string &message)
{
	std::cerr << m_currentFilename << ":" << m_currentLineNo << ": " << message << std::endl;
}

void INIConfig::cleanUpLine(std::string &s)
{
	auto f = [](int c) { return !std::isspace(c); };

	// Left trim
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), f));

	// Remove inline comments, but not if they're in a string
	for (std::size_t i = 0; i < s.length(); ++i) {
		// We found a quote
		if (s[i] == '"' || s[i] == '\'') {
			// Is there a backslash before it?
			if (i > 0 && s[i - 1] != '\\') {
				// Save quote character
				char q = s[i++];

				// Search for quote end
				std::size_t j = i;
				while (j < s.length() && (s[j] != q && s[j - 1] != '\\'))
					j++;

				// If quote has an end, jump to it
				if (j < s.length())
					i = j;
			}
		}
		// We found a comment
		else if (s[i] == '#' || s[i] == ';') {
			s.erase(i);
			break;
		}
	}

	// Right trim
	s.erase(std::find_if(s.rbegin(), s.rend(), f).base(), s.end());
}

std::string INIConfig::getQuotedText(std::string &line, const std::string &quotes)
{
	char quoteBegin = quotes[0];
	char quoteEnd = (quotes.length() > 1) ? quotes[1] : quoteBegin;

	std::size_t begin = 0;
	while (line[begin++] != quoteBegin);

	std::size_t end = begin;
	while (line[end++] != quoteEnd);

	return line.substr(begin, end - begin - 1);
}

void INIConfig::parseExpression(std::string &line)
{
	std::regex sectionRegex("^\\[([\\w]+)\\]$");
	std::regex assignRegex("^([\\w]+)(\\[\\])?\\s=\\s(.+)$");
	std::smatch match;

	if (std::regex_search(line, match, sectionRegex)) {
		m_currentSectionName = match[1];
	}
	else if (std::regex_search(line, match, assignRegex)) {
		if (match.length(2) > 0) { // has prefix '[]'
			if (m_values[m_currentSectionName][match[1]].length() > 0)
				m_values[m_currentSectionName][match[1]] += "\n";
			m_values[m_currentSectionName][match[1]] += match[3];
		}
		else {
			m_values[m_currentSectionName][match[1]] = match[3];
		}
	}
}