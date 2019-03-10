/*
** EPITECH PROJECT, 2019
** zia
** File description:
** HTTPModule.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace Modules {
	class HTTPMod;
}

////////////////////////////////////////////////////////////////////////////////

#include "Module.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace Modules
{

	class EXPORT HTTPMod : public AModule
	{
	public:
		HTTPMod(ModuleLoader *ml);
		~HTTPMod();

		bool onReceive(const json &host, std::shared_ptr<Net::TcpSocket>, std::string &);
		bool onParsing(const json &host, const std::string &buffer, HTTP::Request &req);
		bool onContentGen(const json &host, HTTP::Request &req, HTTP::Response &res);
		bool onSend(const json &host, std::shared_ptr<Net::TcpSocket>, const std::string &);

		// Unused
		bool onConnection(const json &, std::shared_ptr<Net::TcpSocket>) { return false; }
		bool checkModule() { return false; }
	};

}