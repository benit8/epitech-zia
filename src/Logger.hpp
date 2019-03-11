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
#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>

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

public:
	Logger(std::ostream &stream, LogLevel level)
	: lock(s_mutex)
	, stream(&stream)
	, level(level)
	, levelPrinted(false)
	{}

	Logger(Logger &&other)
	: lock(std::move(other.lock))
	, stream(other.stream)
	, level(other.level)
	, levelPrinted(other.levelPrinted)
	{
		other.stream = nullptr;
	}

	friend Logger &&operator << (Logger &&l, std::ostream &(*arg)(std::ostream &))
	{
		if (!l.levelPrinted) {
			(*l.stream) << levelString(l.level);
			l.levelPrinted = true;
		}

		std::ostringstream oss;
		oss << arg;
		(*l.stream) << prettyPrint(oss.str(), l.level);
		return std::move(l);
	}

	template <typename Arg>
	friend Logger &&operator << (Logger &&l, Arg &&arg)
	{
		if (!l.levelPrinted) {
			(*l.stream) << levelString(l.level);
			l.levelPrinted = true;
		}

		std::ostringstream oss;
		oss << std::forward<Arg>(arg);
		(*l.stream) << prettyPrint(oss.str(), l.level);
		return std::move(l);
	}

	static Logger debug() { return Logger(std::cout, Debug); }
	static Logger info() { return Logger(std::cout, Info); }
	static Logger warning() { return Logger(std::cerr, Warning); }
	static Logger error() { return Logger(std::cerr, Error); }

private:
	static std::string prettyPrint(std::string data, LogLevel lvl)
	{
		std::string pad = levelPadding(lvl);
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

	static std::string levelString(LogLevel lvl)
	{
		switch (lvl) {
			default:
			case Debug:	return "  \x1b[35mDEBUG\x1b[0m ";
			case Info:	return "   \x1b[36mINFO\x1b[0m ";
			case Warning:	return "\x1b[33mWARNING\x1b[0m ";
			case Error:	return "  \x1b[31mERROR\x1b[0m ";
		}
	}
	static std::string levelPadding(LogLevel lvl)
	{
		switch (lvl) {
			default:
			case Debug:	return "      \x1b[35m|\x1b[0m ";
			case Info:	return "     \x1b[36m|\x1b[0m ";
			case Warning:	return "        \x1b[33m|\x1b[0m ";
			case Error:	return "      \x1b[31m|\x1b[0m ";
		}
	}

private:
	std::unique_lock<std::mutex> lock;
	std::ostream *stream;
	LogLevel level;
	bool levelPrinted;

	static std::mutex s_mutex;
};