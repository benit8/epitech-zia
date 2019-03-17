/*
** EPITECH PROJECT, 2019
** zia
** File description:
** IModule.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

class IModule;
class AModule;

////////////////////////////////////////////////////////////////////////////////

#include "json.hpp"
#include "Export.hpp"
#include "HTTP.hpp"
#include "Logger.hpp"
#include "ModuleLoader.hpp"
#include "Network/TcpSocket.hpp"

#include <string>

using namespace nlohmann;

////////////////////////////////////////////////////////////////////////////////

class IModule
{
public:
	virtual bool onConnection(json &, Net::TcpSocket &) = 0;
	virtual bool onReceive(json &, Net::TcpSocket &, std::string &) = 0;
	virtual bool onParsing(json &, const std::string &, HTTP::Request &) = 0;
	virtual bool onContentGen(json &, HTTP::Request &, HTTP::Response &) = 0;
	virtual bool onSend(json &, Net::TcpSocket &, const std::string &) = 0;
	virtual bool checkModule() = 0;

	virtual const std::string &getName() = 0;
};

////////////////////////////////////////////////////////////////////////////////

class AModule : public IModule
{
public:
	AModule(ModuleLoader *ml, const std::string &name)
	: m_ml(ml)
	, m_name(name)
	{}

	virtual ~AModule()
	{}

	virtual bool onConnection(json &, Net::TcpSocket &) { return false; }
	virtual bool onReceive(json &, Net::TcpSocket &, std::string &) { return false; }
	virtual bool onParsing(json &, const std::string &, HTTP::Request &) { return false; }
	virtual bool onContentGen(json &, HTTP::Request &, HTTP::Response &) { return false; }
	virtual bool onSend(json &, Net::TcpSocket &, const std::string &) { return false; }
	virtual bool checkModule() { return false; }

public:
	const std::string &getName() { return m_name; }

protected:
	ModuleLoader *m_ml;
	std::string m_name;
};