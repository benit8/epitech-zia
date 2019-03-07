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

#include "HTTP.hpp"
#include "Network/TcpSocket.hpp"

#include <string>

////////////////////////////////////////////////////////////////////////////////

class IModule
{
public:
	virtual ~IModule() = default;

	virtual bool onConnection(Net::TcpSocket *) = 0;
	virtual bool onReceive(Net::TcpSocket *, std::string &) = 0;
	virtual bool onParsing(const std::string &, HTTP::Request &) = 0;
	virtual bool onContentGen(HTTP::Request &, HTTP::Response &) = 0;
	virtual bool onSend(Net::TcpSocket *, const std::string &) = 0;
        virtual bool checkModule() = 0;

	virtual const std::string &getName() = 0;
};

////////////////////////////////////////////////////////////////////////////////

class AModule : public IModule
{
public:
	AModule(const std::string &name)
	: m_name(name)
	{}

	virtual ~AModule()
	{}

public:
	const std::string &getName() { return m_name; }

private:
	std::string m_name;
};
