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
#include "ModuleLoader.hpp"
#include "Network/TcpSocket.hpp"

#include <string>

using namespace nlohmann;

////////////////////////////////////////////////////////////////////////////////

class IModule
{
public:
	virtual ~IModule() = default;

	virtual bool onConnection(const json &, std::shared_ptr<Net::TcpSocket>) = 0;
	virtual bool onReceive(const json &, std::shared_ptr<Net::TcpSocket>, std::string &) = 0;
	virtual bool onParsing(const json &, const std::string &, HTTP::Request &) = 0;
	virtual bool onContentGen(const json &, HTTP::Request &, HTTP::Response &) = 0;
	virtual bool onSend(const json &, std::shared_ptr<Net::TcpSocket>, const std::string &) = 0;
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

public:
	const std::string &getName() { return m_name; }

protected:
	ModuleLoader *m_ml;
	std::string m_name;
};