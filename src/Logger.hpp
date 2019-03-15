/*
** EPITECH PROJECT, 2019
** zia
** File description:
** Logger.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

class Logger;

////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////

class Logger
{
public:
	enum LogLevel
	{
		Debug,
		Info,
		Warning,
		Error
	};

	struct LogLevelParam
	{
		std::string color;
		std::string name;
		std::string padding;
	};

public:
	Logger(std::ostream &stream, LogLevel level);
	Logger(Logger &&other);

public:
	friend Logger &&operator << (Logger &&l, std::ostream &(*arg)(std::ostream &))
	{
		if (!l.levelPrinted) {
			(*l.stream) << s_logLevelParams[l.level].color << s_logLevelParams[l.level].name << s_colorReset;
			l.levelPrinted = true;
		}

		std::ostringstream oss;
		oss << arg;
		std::string buffer = oss.str();
		(*l.stream) << prettyPrint(buffer, l.level);
		return std::move(l);
	}

	template <typename Arg>
	friend Logger &&operator << (Logger &&l, Arg &&arg)
	{
		if (!l.levelPrinted) {
			(*l.stream) << s_logLevelParams[l.level].color << s_logLevelParams[l.level].name << s_colorReset;
			l.levelPrinted = true;
		}

		std::ostringstream oss;
		oss << std::forward<Arg>(arg);
		std::string buffer = oss.str();
		(*l.stream) << prettyPrint(buffer, l.level);
		return std::move(l);
	}

public:
	static Logger debug() { return Logger(std::cout, Debug); }
	static Logger info() { return Logger(std::cout, Info); }
	static Logger warning() { return Logger(std::cerr, Warning); }
	static Logger error() { return Logger(std::cerr, Error); }

private:
	static std::string &prettyPrint(std::string &data, LogLevel lvl);

private:
	std::unique_lock<std::mutex> lock;
	std::ostream *stream;
	LogLevel level;
	bool levelPrinted;

	static std::mutex s_mutex;
	static std::map<LogLevel, LogLevelParam> s_logLevelParams;
	static const char *s_colorReset;
};