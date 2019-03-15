/*
** EPITECH PROJECT, 2019
** zia
** File description:
** Logger.cpp
*/

#include "Logger.hpp"

////////////////////////////////////////////////////////////////////////////////

std::mutex Logger::s_mutex{};

std::map<Logger::LogLevel, Logger::LogLevelParam> Logger::s_logLevelParams = {
	{Debug,   {"\x1b[35m", "  DEBUG ", "      | "}},
	{Info,    {"\x1b[36m", "   INFO ", "      | "}},
	{Warning, {"\x1b[33m", "WARNING ", "      | "}},
	{Error,   {"\x1b[31m", "  ERROR ", "      | "}}
};

const char *Logger::s_colorReset = "\x1b[0m";

////////////////////////////////////////////////////////////////////////////////

Logger::Logger(std::ostream &stream, LogLevel level)
: lock(s_mutex)
, stream(&stream)
, level(level)
, levelPrinted(false)
{}

Logger::Logger(Logger &&other)
: lock(std::move(other.lock))
, stream(other.stream)
, level(other.level)
, levelPrinted(other.levelPrinted)
{
	other.stream = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

std::string &Logger::prettyPrint(std::string &data, Logger::LogLevel lvl)
{
	std::string pad = s_logLevelParams[lvl].color + s_logLevelParams[lvl].padding + s_colorReset;
	data.erase(std::remove(data.begin(), data.end(), '\r'), data.end());

	std::size_t pos = 0;
	while ((pos = data.find('\n', pos)) != std::string::npos) {
		if (pos == data.length() - 1)
			break;
		data.insert(pos + 1, pad);
		pos += pad.length() + 1;
	}

	return data;
}